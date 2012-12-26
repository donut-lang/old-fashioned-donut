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

Sprite::Sprite(HandlerW<internal::SpriteManager> mgr, geom::IntVector const& size)
:mgr_(mgr)
,origSize_(geom::IntBox(getPower2Of(size.width()), getPower2Of(size.height())))
,size_(origSize_)
,texId_(MAGIC)
,locked_(false)
,buffer_(nullptr)
,bufferType_(Sprite::BufferType::Invalid)
{
	glGenTextures(1, &this->texId_);
	const GLenum gerr = glGetError();
	if(gerr != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to generate texture: 0x%08x", gerr);
	}
	glBindTexture(GL_TEXTURE_2D, this->texId_);
	const GLenum berr = glGetError();
	if(berr != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to bind texture: 0x%08x", berr);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->origSize().width(), this->origSize().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	const GLenum terr = glGetError();
	if(terr != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to transfer texture: 0x%08x", terr);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
Sprite::~Sprite() noexcept (true)
{
	this->backBuffer();
	if(this->texId_ != MAGIC){
		glDeleteTextures(1, &this->texId_);
	}
}

void Sprite::drawImpl(Canvas* const canvas, geom::Point const& pt, geom::Area const& renderArea, Color const& color, const float depth)
{
	this->flushBuffer();
	canvas->drawTexture(this->texId_, pt, this->origSize(), renderArea, depth, color);
}
void Sprite::drawImpl(Canvas* const canvas, geom::Point const& pt, Color const& color, const float depth)
{
	this->flushBuffer();
	canvas->drawTexture(this->texId_, pt, this->origSize(), this->size(), depth, color);
}

void Sprite::flushBuffer()
{
	if(this->buffer_){
		{
			glBindTexture(GL_TEXTURE_2D, this->texId_);
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to bind texture: code 0x%x", err);
			}
		}
		{
			//ここのサイズはバッファのものにしないと変な所を読みに行くかもしれない。
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, this->size().width(), this->size().height(), this->bufferType_, GL_UNSIGNED_BYTE, this->buffer_->ptr());
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to transfer texture: code 0x%x", err);
			}
		}
		this->backBuffer();
	}
}

void Sprite::resize(int width, int height)
{
	if(width > this->origSize().width() || height > this->origSize().height()){
		TARTE_EXCEPTION(Exception, "[BUG] You can't resize Sprite bigger than original.");
	}
	this->size(geom::IntBox(width, height));
}

std::string Sprite::toString() const noexcept
{
	return ::tarte::format("(Sprite %p tex: %d buffer: %p orig: %s now: %s)", this, this->texId_, this->buffer_, this->origSize().toString().c_str(), this->size().toString().c_str());
}

//-----------------------------------------------------------------------------
void Sprite::backBuffer()
{
	if(this->buffer_){
		if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
			mgr->backBuffer(this->buffer_);
		}else{
			delete this->buffer_;
		}
		this->buffer_ = nullptr;
	}
}

bool Sprite::onFree() noexcept {
	this->size(this->origSize());
	this->backBuffer();
	if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
		mgr->backSprite(this);
		return true;
	}else{
		return false;
	}
}

internal::Buffer* Sprite::lock(Sprite::BufferType bufferType)
{
	bool expected = false;
	if(!this->locked_.compare_exchange_strong(expected, true)){
		TARTE_EXCEPTION(Exception, "[BUG] Sprite already locked!");
	}
	if(this->buffer_ && this->bufferType_ == bufferType){
		return this->buffer_;
	}else{
		this->flushBuffer();
		if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
			this->buffer_ = mgr->queryBuffer(this->size().width() * this->size().height() * 4);
		}else{
			TARTE_EXCEPTION(Exception, "[BUG] SpriteManager already dead!!");
		}
		this->bufferType_ = bufferType;
		return (this->buffer_);
	}
}
void Sprite::unlock()
{
	bool expected = true;
	if(!this->locked_.compare_exchange_strong(expected, false)){
		TARTE_EXCEPTION(Exception, "[BUG] Sprite already unlocked!");
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

}}
