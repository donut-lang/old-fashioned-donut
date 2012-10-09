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
#include "../tk/Geom.h"
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
,width_(NAN)
,height_(NAN)
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

void Canvas::resize2d(const float width, const float height)
{
	this->width_ = width;
	this->height_ = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glScissor(0,0,width,height);
}

void Canvas::scissor(const float x, const float y, const float width, const float height)
{
	glScissor(x, this->height_-height-y,width, height);
}

void Canvas::scissor(const tk::Area& area)
{
	this->scissor(area.x(), area.y(), area.width(), area.height());
}

void Canvas::scissorReset()
{
	glScissor(0,0,this->width_,this->height_);
}

void Canvas::drawSprite(const float x,const float y, const float z, Handler<RawSprite> sprite)
{
	const GLint texId = sprite->requestTexture();
	const float width = sprite->width();
	const float height = sprite->height();
	const float right = width/sprite->origWidth();
	const float bottom = height/sprite->origHeight();
	glBindTexture(GL_TEXTURE_2D, texId);
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
		glTexCoord2f(0,		0);		glVertex3f(x      , y, z);
		glTexCoord2f(0,		bottom);	glVertex3f(x      , y+height,z);
		glTexCoord2f(right,	bottom);	glVertex3f(x+width, y+height,z);
		glTexCoord2f(right,	0);		glVertex3f(x+width, y,z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFlush();
}

Handler<RawSprite> Canvas::queryRawSprite(const int width, const int height)
{
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), std::pair<int,int>(width,height), Order<RawSprite>());
	if(it == this->unusedSprite_.end()){
		RawSprite* spr = new RawSprite(this, width, height);
		spr->resize(width, height);
		return Handler<RawSprite>(spr);
	}else{
		(*it)->resize(width, height);
		return Handler<RawSprite>(*it);
	}
}

void Canvas::backSprite(RawSprite* spr)
{
	auto ins = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), spr, Order<RawSprite>());
	this->unusedSprite_.insert(ins, spr);
	while(Canvas::MaxCachedSpriteCount < this->unusedSprite_.size()){
		RawSprite* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedSprite_.back();
			this->unusedSprite_.pop_back();
		}else{
			deleted = this->unusedSprite_.front();
			this->unusedSprite_.pop_front();
		}
		if(deleted){
			RawSprite* const min = this->unusedSprite_.front();
			RawSprite* const max = this->unusedSprite_.back();
			if(log().d()){
				log().d(TAG, "Sprite cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}

Buffer* Canvas::queryBuffer(const int width, const int height)
{
	int const pHeight = getPower2Of(height);
	auto it = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), std::pair<int,int>(width,pHeight), Order<Buffer>());
	if(it == this->unusedBuffer_.end() || (*it)->width() != width ){
		return new Buffer(width, pHeight);
	}else{
		return *it;
	}
}

void Canvas::backBuffer(Buffer* buffer)
{
	auto ins = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), buffer, Order<Buffer>());
	this->unusedBuffer_.insert(ins, buffer);
	while(Canvas::MaxCachedBufferCount < this->unusedBuffer_.size()){
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
			if(log().d()){
				log().d(TAG, "Buffer cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}

Canvas::Scissor::Scissor(Canvas& canvas, const tk::Area& area)
:canvas_(canvas)
{
	this->canvas_.scissor(area);
}

Canvas::Scissor::~Scissor()
{
	this->canvas_.scissorReset();
}

}
}

