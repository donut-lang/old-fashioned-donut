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

Sprite::Sprite(SpriteManager* const mgr, const geom::IntVector& size)
:mgr_(mgr)
,origSize_(geom::IntBox(getPower2Of(size.width()), getPower2Of(size.height())))
,size_(origSize_)
,texId_(MAGIC)
,locked_(false)
,buffer_(nullptr)
,bufferType_(Sprite::BufferType::Invalid)
{
	glGenTextures(1, &this->texId_);
	glBindTexture(GL_TEXTURE_2D, this->texId_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->origSize().width(), this->origSize().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed to transfer texture: %d", err);
	}
}
Sprite::~Sprite() noexcept (true)
{
	this->flushBuffer();
	if(this->texId_ != MAGIC){
		glDeleteTextures(1, &this->texId_);
	}
}

void Sprite::drawImpl(Canvas* const canvas, const geom::Point& pt, const geom::Area& renderArea, const float depth)
{
	this->flushBuffer();
	canvas->drawTexture(this->texId_, pt, this->origSize(), renderArea, depth);
}
void Sprite::drawImpl(Canvas* const canvas, const geom::Point& pt, const float depth)
{
	this->flushBuffer();
	canvas->drawTexture(this->texId_, pt, this->origSize(), this->size(), depth);
}

void Sprite::flushBuffer()
{
	if(this->buffer_){
		glBindTexture(GL_TEXTURE_2D, this->texId_);
		//ここのサイズはバッファのものにしないと変な所を読みに行くかもしれない。
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, this->size().width(), this->size().height(), this->bufferType_, GL_UNSIGNED_BYTE, this->buffer_->ptr());
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
}

void Sprite::resize(int width, int height)
{
	if(width > this->origSize().width() || height > this->origSize().height()){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] You can't resize Sprite bigger than original.");
	}
	this->size(geom::IntBox(width, height));
}

//-----------------------------------------------------------------------------

void Sprite::increfImpl() noexcept
{
	std::unique_lock<std::mutex> lock(this->ref_mutex_);
	this->HandlerBody<Sprite>::increfImpl();
}
void Sprite::decrefImpl()
{
	std::unique_lock<std::mutex> lock(this->ref_mutex_);
	this->HandlerBody<Sprite>::decrefImpl();
}

void Sprite::onFree() noexcept {
	this->size(this->origSize());
	if(this->buffer_){
		this->mgr_->backBuffer(this->buffer_);
		this->buffer_ = nullptr;
	}
	this->mgr_->backSprite(this);
}

internal::Buffer* Sprite::lock(Sprite::BufferType bufferType)
{
	bool expected = false;
	if(!this->locked_.compare_exchange_strong(expected, true)){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite already locked!");
	}
	if(this->buffer_ && this->bufferType_ == bufferType){
		return this->buffer_;
	}else{
		this->flushBuffer();
		this->buffer_ = this->mgr_->queryBuffer(this->size().width() * this->size().height() * 4);
		this->bufferType_ = bufferType;
		return (this->buffer_);
	}
}
void Sprite::unlock()
{
	bool expected = true;
	if(!this->locked_.compare_exchange_strong(expected, false)){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite already unlocked!");
	}
}

//-----------------------------------------------------------------------------

Sprite::Session::Session(Handler<Sprite> parent, Sprite::BufferType bufferType)
:parent_(parent)
{
	this->parent_->lock(bufferType);
}
Sprite::Session::~Session()
{
	this->parent_->unlock();
}

//----------------------------------------------------------------------------

static const std::string TAG("RawSpriteManager");

SpriteManager::SpriteManager(logging::Logger& log)
:log_(log)
{

}
SpriteManager::~SpriteManager() noexcept
{
	for(internal::Buffer* b : this->unusedBuffer_){
		delete b;
	}
}

void SpriteManager::backSprite(Sprite* spr)
{
	typedef chisa::gl::internal::WidthOrder<Sprite> SpriteOrder;
	auto ins = std::upper_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), spr, SpriteOrder());
	this->unusedSprite_.insert(ins, spr);
	while(MaxCachedSpriteCount < this->unusedSprite_.size()){
		Sprite* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedSprite_.back();
			this->unusedSprite_.pop_back();
		}else{
			deleted = this->unusedSprite_.front();
			this->unusedSprite_.	pop_front();
		}
		if(deleted){
			Sprite* const min = this->unusedSprite_.front();
			Sprite* const max = this->unusedSprite_.back();
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

Handler<Sprite> SpriteManager::queryRawSprite(const int width, const int height)
{
	typedef chisa::gl::internal::WidthOrder<Sprite> SpriteOrder;
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), std::pair<int,int>(width,height), SpriteOrder());
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedSprite_.end() || (*it)->height() < height){
		Handler<Sprite> spr ( new Sprite(this, geom::IntBox(width, height)) );
		spr->resize(width, height);
		return spr;
	}else{
		Handler<Sprite> spr(*it);
		this->unusedSprite_.erase(it);
		spr->resize(width, height);
		return spr;
	}
}

internal::Buffer* SpriteManager::queryBuffer(const std::size_t size)
{
	typedef chisa::gl::internal::SizeOrder<internal::Buffer> BufferOrder;
	auto it = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), size, BufferOrder());
	if(it == this->unusedBuffer_.end() || (*it)->size() >= size*2){
		return new internal::Buffer(size);
	}else{
		internal::Buffer* const buf = *it;
		this->unusedBuffer_.erase(it);
		return buf;
	}
}

void SpriteManager::backBuffer(internal::Buffer* buffer)
{
	typedef chisa::gl::internal::SizeOrder<internal::Buffer> BufferOrder;
	auto ins = std::upper_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), buffer, BufferOrder());
	this->unusedBuffer_.insert(ins, buffer);
	while(MaxCachedBufferCount < this->unusedBuffer_.size()){
		internal::Buffer* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedBuffer_.back();
			this->unusedBuffer_.pop_back();
		}else{
			deleted = this->unusedBuffer_.front();
			this->unusedBuffer_.pop_front();
		}
		if(deleted){
			internal::Buffer* const min = this->unusedBuffer_.front();
			internal::Buffer* const max = this->unusedBuffer_.back();
			if(this->log().d()){
				this->log().d(TAG, "Buffer cache deleted. size: %d / min:%d, max:%d",
						deleted->size(), min->size(), max->size());
			}
			delete deleted;
		}
	}
}


//-----------------------------------------------------------------------------
namespace internal {
Buffer::Buffer(std::size_t size)
:size_(size), ptr_(new unsigned char [size])
{

}
Buffer::~Buffer() noexcept
{
	delete [] ptr_;
}


}
}}
