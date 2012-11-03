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
#include "OpenGL.h"
#include "../logging/Logger.h"
#include "../util/ClassUtil.h"
#include "../util/Thread.h"
#include "../Handler.h"
#include "../geom/Area.h"
#include "internal/SpriteManager.h"

namespace chisa {
namespace gl {

class Sprite : public HandlerBody<Sprite, true> {
	HandlerW<internal::SpriteManager> mgr_;
	DEFINE_MEMBER(public, private, geom::IntVector, origSize);
	DEFINE_MEMBER(public, private, geom::IntVector, size);
	unsigned int texId_;
	std::atomic<bool> locked_;
public:
	Sprite(HandlerW<internal::SpriteManager> mgr, const geom::IntVector& size);
	virtual ~Sprite() noexcept(true);
	enum BufferType {
		Invalid = 0,
		RGBA8 = GL_RGBA,
		BGRA8 = GL_BGRA
	};
private:
	internal::Buffer* lock(BufferType type);
	void unlock();
	void flushBuffer();
	void backBuffer();
private:
	internal::Buffer* buffer_;
	BufferType bufferType_;
public:
	class Session {
		DISABLE_COPY_AND_ASSIGN(Session);
		STACK_OBJECT(Session);
	private:
		Handler<Sprite> parent_;
	public:
		Session(Handler<Sprite> parent, Sprite::BufferType bufferType);
		~Session();
		inline int width() const noexcept { return parent_->size().width(); };
		inline int height() const noexcept { return parent_->size().height(); };
		inline geom::IntBox size() const noexcept { return parent_->size(); };
		inline int stride() const noexcept { return parent_->size().width() * 4; };
		inline unsigned char* data() const noexcept { return parent_->buffer_->ptr(); };
	};
public:
	void resize(int width, int height);
	inline int width() const noexcept { return this->size().width(); };
	inline int height() const noexcept { return this->size().height(); };
	std::string toString() const noexcept;
public: /* from Handler */
	void onFree() noexcept;
public: /* from Canvas */
	void drawImpl(Canvas* const canvas, const geom::Point& pt, const geom::Area& renderArea, const float depth);
	void drawImpl(Canvas* const canvas, const geom::Point& pt, const float depth);
};

}}
