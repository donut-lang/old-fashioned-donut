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

#pragma once
#include <tarte/Logger.h>
#include <tarte/ClassUtil.h>
#include <tarte/Thread.h>
#include <tarte/Handler.h>
#include "OpenGL.h"
#include "Color.h"
#include "ImageFormat.h"
#include "../geom/Area.h"
#include "internal/Buffer.h"
#include "internal/SpriteManager.h"

namespace chisa {
using namespace tarte;
namespace gl {
class Canvas;

class Texture {
public:
	STACK_OBJECT(Texture);
	DISABLE_COPY_AND_ASSIGN(Texture);
private:
	unsigned int const texId_;
	ImageFormat const format_;
	int const width_;
	int const height_;
private:
	int const align_;
public:
	Texture(unsigned int const texId, ImageFormat const format, int const width, int const height)
	:texId_(texId), format_(format), width_(width), height_(height)
	,align_(Texture::formatToAlign(format))
	{
	}
public:
	static inline int formatToAlign(ImageFormat const format) noexcept{
		switch( format ) {
		case ImageFormat::RGBA8:
			return 4;
		case ImageFormat::BGRA8:
			return 4;
		case ImageFormat::ALPHA:
			return 1;
		default:
			return 0;
		}
	}
public:
	inline const unsigned int& textureID() const noexcept { return this->texId_; };
	inline const int& width() const noexcept { return this->width_; };
	inline const int& height() const noexcept { return this->height_; };
	inline const ImageFormat& format() const noexcept { return this->format_; };
	inline const int& align() const noexcept { return this->align_; };
public:
	typedef std::tuple<ImageFormat,int,int> Key;
	friend bool operator<(Texture const& a,Texture const& b) noexcept;
	friend bool operator<(Texture const& a,Texture::Key const& b) noexcept;
	friend bool operator<(Texture::Key const& a,Texture const& b) noexcept;
};

inline bool operator<(Texture const& a,Texture const& b) noexcept{
	return std::tie( a.format_, a.width_, a.height_ ) < std::tie(b.format_, b.width_, b.height_);
}
inline bool operator<(Texture const& a,Texture::Key const& b) noexcept{
	return std::tie( a.format_, a.width_, a.height_ ) < b;
}
inline bool operator<(Texture::Key const& a,Texture const& b) noexcept{
	return a < std::tie( b.format_, b.width_, b.height_ );
}

class Sprite : public HandlerBody<Sprite, true> {
private:
	HandlerW<internal::SpriteManager> mgr_;
private:
	Texture texture_;
private:
	DEFINE_MEMBER(public, private, geom::IntVector, size);
	std::atomic<bool> locked_;
	struct {
		internal::Buffer* mem_;
		unsigned int width_;
		unsigned int height_;
		unsigned int stride_;
		ImageFormat format_;
	} buffer_;
public:
	Sprite(HandlerW<internal::SpriteManager> mgr, ImageFormat format, geom::IntVector const& size);
	virtual ~Sprite() noexcept(true);
public:
	inline Texture const& texture() const noexcept { return this->texture_; };
private:
	internal::Buffer* lock(ImageFormat format);
	void unlock();
	void flushBuffer();
	void backBuffer();
public:
	class Session {
		DISABLE_COPY_AND_ASSIGN(Session);
		STACK_OBJECT(Session);
	private:
		Handler<Sprite> parent_;
	public:
		Session(Handler<Sprite> parent, ImageFormat bufferType=ImageFormat::Invalid);
		~Session();
		inline int width() const noexcept { return parent_->size_.width(); };
		inline int height() const noexcept { return parent_->size_.height(); };
		inline geom::IntBox size() const noexcept { return parent_->size_; };
		inline int stride() const noexcept { return parent_->buffer_.stride_; };
		inline unsigned char* data() const noexcept { return parent_->buffer_.mem_->ptr(); };
	};
public:
	void resize(int width, int height);
	inline int width() const noexcept { return this->size().width(); };
	inline int height() const noexcept { return this->size().height(); };
	std::string toString() const noexcept;
public: /* from Handler */
	bool onFree() noexcept;
public: /* from Canvas */
	void drawImpl(Canvas* const canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth, Color const& color);
	void drawImpl(Canvas* const canvas, geom::Point const& ptInRoot, const float depth, Color const& color);
public:
	struct CompareByTexture{
		inline bool operator()(Sprite const* const& a, Sprite const* const& b) const noexcept
		{
			return a->texture_ < b->texture_;
		}
		inline bool operator() (Sprite const* const& a, std::tuple<ImageFormat,int,int> const& b) const noexcept
		{
			return a->texture_ < b;
		}
		inline bool operator() (std::tuple<ImageFormat,int,int> const& a, Sprite const* const& b) const noexcept
		{
			return a < b->texture_;
		}
	};
};

}}
