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
#include "../util/class_utils.h"
#include "Handler.h"

namespace chisa {
namespace gl {

class Canvas;

class Sprite {
	DISABLE_COPY_AND_ASSIGN(Sprite);
private:
	DEFINE_MEMBER_CONST(public, Canvas*, canvas);
	int refcount_;
	DEFINE_MEMBER(public, private, int, origWidth);
	DEFINE_MEMBER(public, private, int, origHeight);
	DEFINE_MEMBER(public, private, int, width);
	DEFINE_MEMBER(public, private, int, height);
	unsigned char* data_;
	unsigned int texId_;
	bool dirty_;
	DEFINE_MEMBER(private, private, bool, locked);
public:
	Sprite(Canvas* const canvas, const int width, const int height);
	virtual ~Sprite();
private:
	void resize(int width, int height);
	void incref();
	void decref();
	void lock(unsigned char** data, int* stride);
	void unlock();
private:
	unsigned char* requestMemory();
public:
	unsigned int requestTexture();
public:
	class Handler : public chisa::gl::Handler<Sprite> {
	public:
		Handler();
		Handler(Sprite* const obj, int width, int height);
		virtual ~Handler();
	};
	class Session {
		DISABLE_COPY_AND_ASSIGN(Session);
	private:
		Sprite::Handler parent_;
		int stride_;
		unsigned char* data_;
	public:
		Session(Sprite::Handler parent);
		virtual ~Session();
		inline int width() const { return parent_->width(); };
		inline int height() const { return parent_->height(); };
		inline int stride() const { return this->stride_; };
		inline unsigned char* data() const { return this->data_; };
	};
	friend class gl::Handler<Sprite>;
};

}}
#endif /* INCLUDE_GUARD */
