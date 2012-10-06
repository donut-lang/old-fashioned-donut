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

#ifndef Chisa_CANVAS_H__CPP_
#define Chisa_CANVAS_H__CPP_

#include "../logging/Exception.h"
#include "../logging/Logger.h"
#include "../util/class_utils.h"
#include "Handler.h"
#include "Sprite.h"
#include <deque>

namespace chisa {
namespace gl {

class Canvas {
	DISABLE_COPY_AND_ASSIGN(Canvas);
private:
	static constexpr const size_t MaxCachedBufferCount = 200;
	static constexpr const size_t MaxCachedSpriteCount = 200;
private:
	logging::Logger& log_;
public:
	Canvas(logging::Logger& log);
	virtual ~Canvas();
public:
	std::deque<Sprite*> unusedSprite_;
public:
	void ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val);
	void pushMatrix();
	void popMatrix();
	void translate(const float x,const float y, const float z);
	void rotate(const float angle, const float x,const float y, const float z);
	void scale(const float x,const float y, const float z);
	void drawSprite(const float x,const float y, const float z, Sprite::Handler sprite);
public:
	Sprite::Handler loadPNG(const std::string& filename);
	Sprite::Handler querySprite(const int width, const int height);
public:
	void backSprite(Sprite* spr);
};

}}
#endif /* INCLUDE_GUARD */
