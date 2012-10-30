/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OpenGL.h"
#include "Sprite.h"
#include "../geom/Area.h"
#include "Canvas.h"
#include "Util.h"
#include "internal/Order.h"
#include <algorithm>
#include <string>

namespace chisa {
namespace gl {

static constexpr unsigned int MAGIC=0xDEADBEEF;

RawSprite::RawSprite(RawSpriteManager* const mgr, const int width, const int height)
:Sprite()
,mgr_(mgr)
,origWidth_(getPower2Of(width))
,origHeight_(getPower2Of(height))
,width_(origWidth_)
,height_(origHeight_)
,texId_(MAGIC)
,locked_(false)
,buffer_(nullptr)
{
	glGenTextures(1, &this->texId_);
	glBindTexture(GL_TEXTURE_2D, this->texId_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->origWidth(), this->origHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed to transfer texture: %d", err);
	}
}
RawSprite::~RawSprite() noexcept
{
	if(this->texId_ != MAGIC){
		glDeleteTextures(1, &this->texId_);
	}
}

void RawSprite::drawImpl(Canvas* const canvas, const geom::Point& pt, const float depth)
{
	if(this->buffer_){
		glBindTexture(GL_TEXTURE_2D, this->texId_);
		//ここのサイズはバッファのものにしないと変な所を読みに行くかもしれない。
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, this->buffer_->width(), this->height(), GL_RGBA, GL_UNSIGNED_BYTE, this->buffer_->data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed to transfer texture: code 0x%x", err);
		}
		this->mgr_->backBuffer(this->buffer_);
		this->buffer_ = nullptr;
	}
	canvas->drawTexture(this->texId_, geom::Area(this->origWidth_, this->origHeight_, this->width_, this->height_), pt, depth);
}

void RawSprite::resize(int width, int height)
{
	if(width > this->origWidth() || height > this->origHeight()){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] You can't resize Sprite bigger than original.");
	}
	this->width(width);
	this->height(height);
}

//-----------------------------------------------------------------------------

void RawSprite::onFree() noexcept {
	this->width_ = this->origWidth_;
	this->height_ = this->origHeight_;
	if(this->buffer_){
		this->mgr_->backBuffer(this->buffer_);
		this->buffer_ = nullptr;
	}
	this->mgr_->backSprite(this);
}

Buffer* RawSprite::lock()
{
	bool expected = false;
	if(!this->locked_.compare_exchange_strong(expected, true)){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite already locked!");
	}
	if(this->buffer_){
		return this->buffer_;
	}else{
		//横幅はオリジナルでないとテクスチャ転送できないが、縦サイズは何でもよいので最小サイズを指定する
		return (this->buffer_ = this->mgr_->queryBuffer(this->origWidth(), this->height()));
	}
}
void RawSprite::unlock()
{
	bool expected = true;
	if(!this->locked_.compare_exchange_strong(expected, false)){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite already unlocked!");
	}
}

//-----------------------------------------------------------------------------

RawSprite::Session::Session(Handler<RawSprite> parent)
:parent_(parent)
{
	this->parent_->lock();
}
RawSprite::Session::~Session()
{
	this->parent_->unlock();
}

//----------------------------------------------------------------------------

static const std::string TAG("RawSpriteManager");

RawSpriteManager::RawSpriteManager(logging::Logger& log)
:log_(log)
{

}
RawSpriteManager::~RawSpriteManager() noexcept
{
	for(Buffer* b : this->unusedBuffer_){
		delete b;
	}
}

void RawSpriteManager::backSprite(RawSprite* spr)
{
	typedef chisa::gl::internal::WidthOrder<RawSprite> SpriteOrder;
	auto ins = std::upper_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), spr, SpriteOrder());
	this->unusedSprite_.insert(ins, spr);
	while(MaxCachedSpriteCount < this->unusedSprite_.size()){
		RawSprite* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedSprite_.back();
			this->unusedSprite_.pop_back();
		}else{
			deleted = this->unusedSprite_.front();
			this->unusedSprite_.	pop_front();
		}
		if(deleted){
			RawSprite* const min = this->unusedSprite_.front();
			RawSprite* const max = this->unusedSprite_.back();
			if(this->log().d()){
				this->log().d(TAG, "Sprite cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}

Handler<RawSprite> RawSpriteManager::queryRawSprite(const int width, const int height)
{
	typedef chisa::gl::internal::WidthOrder<RawSprite> SpriteOrder;
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), std::pair<int,int>(width,height), SpriteOrder());
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedSprite_.end() || (*it)->height() < height){
		Handler<RawSprite> spr ( new RawSprite(this, width, height) );
		spr->resize(width, height);
		return spr;
	}else{
		Handler<RawSprite> spr(*it);
		this->unusedSprite_.erase(it);
		spr->resize(width, height);
		return spr;
	}
}

Buffer* RawSpriteManager::queryBuffer(const int width, const int height)
{
	typedef chisa::gl::internal::WidthOrder<Buffer> BufferOrder;
	int const pHeight = getPower2Of(height);
	auto it = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), std::pair<int,int>(width,pHeight), BufferOrder());
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedBuffer_.end() || (*it)->width() != width ){
		return new Buffer(width, pHeight);
	}else{
		Buffer* const buf = *it;
		this->unusedBuffer_.erase(it);
		return buf;
	}
}

void RawSpriteManager::backBuffer(Buffer* buffer)
{
	typedef chisa::gl::internal::WidthOrder<Buffer> BufferOrder;
	auto ins = std::upper_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), buffer, BufferOrder());
	this->unusedBuffer_.insert(ins, buffer);
	while(MaxCachedBufferCount < this->unusedBuffer_.size()){
		Buffer* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedBuffer_.back();
			this->unusedBuffer_.pop_back();
		}else{
			deleted = this->unusedBuffer_.front();
			this->unusedBuffer_.pop_front();
		}
		if(deleted){
			Buffer* const min = this->unusedBuffer_.front();
			Buffer* const max = this->unusedBuffer_.back();
			if(this->log().d()){
				this->log().d(TAG, "Buffer cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}


}}
