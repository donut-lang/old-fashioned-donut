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

#include "Canvas.h"
#include <algorithm>
#include <GL/gl.h>
#include <cairo/cairo.h>


template <typename T>
struct Order {
	bool operator()(const T* const a, const T* const b)
	{
		return a->width() <= b->width() && a->height() <= b->height();
	}
	bool operator() (const T* a, const std::pair<int,int>& b)
	{
		return a->width() <= b.first && a->height() <= b.second;
	}
	bool operator() (const std::pair<int,int>& a, const T* b)
	{
		return a.first <= b->width() && a.second <= b->height();
	}
};

namespace chisa {
namespace gl {

const static std::string TAG("Canvas");

Canvas::Canvas(logging::Logger& log)
:log_(log)
{

}

Canvas::~Canvas()
{

}

void Canvas::ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val)
{
	glOrtho(left, right, bottom, top, near_val, far_val);
}
void Canvas::pushMatrix()
{
	glPushMatrix();
}
void Canvas::popMatrix()
{
	glPopMatrix();
}
void Canvas::translate(const float x,const float y, const float z)
{
	glTranslatef(x,y,z);
}
void Canvas::rotate(const float angle, const float x,const float y, const float z)
{
	glRotatef(angle, x,y,z);
}
void Canvas::scale(const float x,const float y, const float z)
{
	glScalef(x,y,z);
}
void Canvas::drawSprite(const float x,const float y, const float z, Sprite::Handler sprite)
{
}

Sprite::Handler Canvas::querySprite(const int width, const int height)
{

}

template <typename T, size_t maxCount>
void backImage(logging::Logger& log_, const char* name, std::deque<T*>& unused, T* t)
{
	auto ins = std::upper_bound(unused.begin(), unused.end(), t);
	unused.insert(ins, t);
	while(maxCount < unused.size()){
		T* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = unused.back();
			unused.pop_back();
		}else{
			deleted = unused.front();
			unused.pop_front();
		}
		if(deleted){
			T* min = unused.front();
			T* max = unused.back();
			if(log_.d()){
				log_.d(TAG, "%s cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						name,
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}

void Canvas::backSprite(Sprite* spr)
{
	backImage<Sprite, Canvas::MaxCachedSpriteCount>(log_, "Sprite", unusedSprite_, spr);
}


}}
