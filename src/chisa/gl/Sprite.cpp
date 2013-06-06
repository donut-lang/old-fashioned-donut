/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include <string>
#include <cstring>
#include "OpenGL.h"
#include "Canvas.h"
#include "Util.h"
#include "Sprite.h"
#include "internal/SpriteManager.h"
#include "internal/Order.h"
#include "../geom/Area.h"

namespace chisa {
namespace gl {

static constexpr unsigned int MAGIC=0xDEADBEEF;

Sprite::Sprite(HandlerW<internal::SpriteManager> mgr, ImageFormat format, geom::IntVector const& size)
:mgr_(mgr)
,texture_(MAGIC, format, getPower2Of(size.width()), getPower2Of(size.height()))
,size_(texture_.width(), texture_.height())
,locked_(false)
{
	buffer_.mem_ = nullptr;
	glGenTextures(1, &const_cast<unsigned int&>(texture_.textureID()));
	const GLenum gerr = glGetError();
	if( unlikely(gerr != GL_NO_ERROR) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Failed to generate texture: 0x%08x", gerr);
	}
	glBindTexture(GL_TEXTURE_2D, texture_.textureID());
	const GLenum berr = glGetError();
	if( unlikely(gerr != GL_NO_ERROR) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Failed to bind texture: 0x%08x", berr);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, this->texture_.align());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->texture_.width(), this->texture_.height(), 0, static_cast<unsigned int>(this->texture_.format()), GL_UNSIGNED_BYTE, nullptr);
	const GLenum terr = glGetError();
	if( unlikely(gerr != GL_NO_ERROR) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Failed to transfer texture: 0x%08x", terr);
	}
}
Sprite::~Sprite() noexcept (true)
{
	this->backBuffer();
	if( likely(this->texture_.textureID() != MAGIC) ){
		glDeleteTextures(1, &(texture_.textureID()));
	}
}

void Sprite::drawImpl(Canvas* const canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth, Color const& color)
{
	this->flushBuffer();
	geom::Area const drawn(geom::Area(geom::ZERO, size_).intersect(mask));
	geom::Area coordinate(drawn);
	coordinate.x(coordinate.x() / this->texture_.width());
	coordinate.y(coordinate.y() / this->texture_.height());
	coordinate.width(coordinate.width() / this->texture_.width());
	coordinate.height(coordinate.height() / this->texture_.height());
	canvas->drawTexture(
			this->texture_.textureID(),
			geom::Area(ptInRoot+drawn.point(), drawn.box()),
			coordinate,
			depth,
			color);
}
void Sprite::drawImpl(Canvas* const canvas, geom::Point const& ptInRoot, const float depth, Color const& color)
{
	this->flushBuffer();
	geom::Area coordinate(geom::Area(geom::ZERO, size_));
	coordinate.width(coordinate.width() / this->texture_.width());
	coordinate.height(coordinate.height() / this->texture_.height());
	canvas->drawTexture(
			this->texture_.textureID(),
			geom::Area(ptInRoot, size_),
			coordinate,
			depth,
			color);
}

void Sprite::drawImpl(Canvas* const canvas, std::vector<float> const& pts, std::vector<float> const texCoords, Color const& color)
{
	this->flushBuffer();
	canvas->drawTexture(
			this->texture_.textureID(),
			pts, texCoords, color);
}

void Sprite::flushBuffer()
{
	if(this->buffer_.mem_){
		{
			glBindTexture(GL_TEXTURE_2D, this->texture_.textureID());
			const GLenum err = glGetError();
			if( unlikely(err != GL_NO_ERROR) ) {
				CINAMO_EXCEPTION(Exception, "[BUG] Failed to bind texture: code 0x%x", err);
			}
		}
		{
			//ここのサイズはバッファのものにしないと変な所を読みに行くかもしれない。
			glTexSubImage2D(
					GL_TEXTURE_2D, 0, 0,0,
					this->buffer_.width_, this->buffer_.height_,
					static_cast<unsigned int>(this->buffer_.format_),
					GL_UNSIGNED_BYTE, this->buffer_.mem_->ptr());
			const GLenum err = glGetError();
			if( unlikely(err != GL_NO_ERROR) ){
				CINAMO_EXCEPTION(Exception, "[BUG] Failed to transfer texture: code 0x%x", err);
			}
		}
		this->backBuffer();
	}
}

void Sprite::resize(int width, int height)
{
	if(unlikely(width > this->texture_.width() || height > this->texture_.height())){
		CINAMO_EXCEPTION(Exception, "[BUG] You can't resize Sprite bigger than original.");
	}
	this->size(geom::IntBox(width, height));
	this->buffer_.width_ = texture_.width();
	this->buffer_.height_ = std::min(height+1, texture_.height());
}

std::string Sprite::toString() const noexcept
{
	return ::cinamo::format("(Sprite %p tex: %d buffer: %p orig: (%dx%d) now: (%dx%d))", this,
			this->texture_.textureID(),
			this->buffer_.mem_,
			this->texture_.width(), texture_.height(),
			this->size().width(),this->size().height());
}

//-----------------------------------------------------------------------------
void Sprite::backBuffer()
{
	if(this->buffer_.mem_){
		if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
			mgr->backBuffer(this->buffer_.mem_);
		}else{
			delete this->buffer_.mem_;
		}
		this->buffer_.mem_ = nullptr;
	}
}

bool Sprite::onFree() noexcept {
	this->size(geom::IntVector(texture_.width(), texture_.height()));
	this->backBuffer();
	if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
		mgr->backSprite(this);
		return true;
	}else{
		return false;
	}
}

internal::Buffer* Sprite::lock(ImageFormat imageFormat)
{
	bool expected = false;
	if(unlikely(!this->locked_.compare_exchange_strong(expected, true))){
		CINAMO_EXCEPTION(Exception, "[BUG] Sprite already locked!");
	}
	if( this->buffer_.mem_ && this->buffer_.format_ == imageFormat ){
		return this->buffer_.mem_;
	} else if( unlikely( this->texture_.align() != Texture::formatToAlign(imageFormat) ) ) {
		CINAMO_EXCEPTION(Exception, "[BUG] Texture align does not match: requested: %d != texture: %d",
				this->texture_.align(), Texture::formatToAlign(imageFormat) );
	} else {
		this->flushBuffer();
		if( Handler<internal::SpriteManager> mgr = this->mgr_.lock() ){
			this->buffer_.stride_ = texture_.width() * texture_.align();
			this->buffer_.mem_ = mgr->queryBuffer(buffer_.stride_ * buffer_.height_);
			std::memset(buffer_.mem_->ptr(), 0, buffer_.mem_->size());
		}else{
			CINAMO_EXCEPTION(Exception, "[BUG] SpriteManager already dead!!");
		}
		this->buffer_.format_ = imageFormat;
		this->buffer_.align_ = Texture::formatToAlign(imageFormat);
		return (this->buffer_.mem_);
	}
}
void Sprite::unlock()
{
	bool expected = true;
	if(unlikely(!this->locked_.compare_exchange_strong(expected, false))){
		CINAMO_EXCEPTION(Exception, "[BUG] Sprite already unlocked!");
	}
}

//-----------------------------------------------------------------------------

Sprite::Session::Session(Handler<Sprite> parent, ImageFormat imageFormat)
:parent_(parent)
{
	this->parent_->lock(imageFormat == ImageFormat::Invalid ? this->parent_->texture_.format() : imageFormat);
}
Sprite::Session::~Session()
{
	this->parent_->unlock();
}

}}
