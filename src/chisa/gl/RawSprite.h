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

#ifndef Chisa_SPRITE_H__CPP_
#define Chisa_SPRITE_H__CPP_

#include <cairo/cairo.h>
#include "../util/ClassUtil.h"
#include "Handler.h"
#include "Buffer.h"
#include "Sprite.h"

namespace chisa {
namespace gl {

class Canvas;

class RawSprite : public Sprite {
	DISABLE_COPY_AND_ASSIGN(RawSprite);
	friend class Canvas;
private:
	DEFINE_MEMBER(public, private, int, origWidth);
	DEFINE_MEMBER(public, private, int, origHeight);
	DEFINE_MEMBER(public, private, int, width);
	DEFINE_MEMBER(public, private, int, height);
	unsigned int texId_;
	DEFINE_MEMBER(private, private, bool, locked);
public:
	RawSprite(Canvas* const canvas, const int width, const int height);
	virtual ~RawSprite();
private:
	void resize(int width, int height);
	Buffer* lock();
	void unlock();
private:
	virtual void onFree() override;
	virtual void drawImpl(const geom::Point& pt, const float depth=0.0f) override;
	unsigned int requestTexture();
	Buffer* buffer_;
public:
	class Session {
		DISABLE_COPY_AND_ASSIGN(Session);
	private:
		Handler<RawSprite> parent_;
	public:
		Session(Handler<RawSprite> parent);
		virtual ~Session();
		inline int width() const { return parent_->width(); };
		inline int height() const { return parent_->height(); };
		inline int stride() const { return parent_->buffer_->stride(); };
		inline unsigned char* data() const { return parent_->buffer_->data(); };
	};
};

}}
#endif /* INCLUDE_GUARD */
