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
#include "../geom/Vector.h"
#include "../geom/Area.h"
#include "Util.h"
#include <algorithm>
#include <GL/gl.h>
#include <cairo/cairo.h>

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
void Canvas::translate(const geom::Point& pt)
{
	glTranslatef(pt.x(),pt.y(),0.0f);
}
void Canvas::rotate(const float angle, const geom::Point& pt)
{
	glRotatef(angle, pt.x(),pt.y(),0.0f);
}
void Canvas::scale(const geom::ScaleVector& scale)
{
	glScalef(scale.x(),scale.y(),1.0f);
}

void Canvas::resize2d(const geom::Box& box)
{
	this->width_ = box.width();
	this->height_ = box.height();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	this->ortho(0, box.width(), box.height(), 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, box.width(), box.height());
	glScissor(0,0,box.width(),box.height());
}

void Canvas::scissor(const geom::Area& area)
{
	glScissor(area.x(), this->height_-area.height()-area.y(),area.width(), area.height());
}

void Canvas::scissorReset()
{
	glScissor(0,0,this->width_,this->height_);
}

void Canvas::drawSprite(Handler<Sprite> sprite, const geom::Point& pt, const float depth)
{
	sprite->drawImpl(pt, depth);
}
void Canvas::drawSprite(RawSprite* const sprite, const geom::Point& pt, const float depth)
{
	const GLint texId = sprite->requestTexture();
	const float width = sprite->width();
	const float height = sprite->height();
	const float right = width/sprite->origWidth();
	const float bottom = height/sprite->origHeight();
	glBindTexture(GL_TEXTURE_2D, texId);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
		glTexCoord2f(0,		0);		glVertex3f(pt.x()      , pt.y(), depth);
		glTexCoord2f(0,		bottom);glVertex3f(pt.x()      , pt.y()+height, depth);
		glTexCoord2f(right,	bottom);glVertex3f(pt.x()+width, pt.y()+height, depth);
		glTexCoord2f(right,	0);		glVertex3f(pt.x()+width, pt.y(),depth);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFlush();
}

Handler<RawSprite> Canvas::queryRawSprite(const int width, const int height)
{
	internal::SpriteOrder order;
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), std::pair<int,int>(width,height), order);
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedSprite_.end() || (*it)->height() < height){
		RawSprite* spr = new RawSprite(this, width, height);
		spr->resize(width, height);
		return Handler<RawSprite>(spr);
	}else{
		(*it)->resize(width, height);
		Handler<RawSprite> spr(*it);
		this->unusedSprite_.erase(it);
		return spr;
	}
}

void Canvas::drawLine(const float width, const Color& color, const geom::Point& start, const geom::Point& end, const float depth)
{
	glLineWidth(width);
	this->setColor(color);
	glBegin(GL_LINES);
		glVertex3f(start.x(), start.y(), depth);
		glVertex3f(end.x()  , end.y(), depth);
	glEnd();
	glFlush();
}

void Canvas::setColor(const Color& color)
{
	glColor4f(color.red(), color.green(), color.blue(), color.alpha());
}

void Canvas::backSprite(RawSprite* spr)
{
	auto ins = std::upper_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), spr, internal::SpriteOrder());
	this->unusedSprite_.insert(ins, spr);
	while(Canvas::MaxCachedSpriteCount < this->unusedSprite_.size()){
		RawSprite* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedSprite_.back();
			this->unusedSprite_.pop_back();
		}else{
			deleted = this->unusedSprite_.front();
			this->unusedSprite_.	pop_front();
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
	internal::BufferOrder order;
	int const pHeight = getPower2Of(height);
	auto it = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), std::pair<int,int>(width,pHeight), order);
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedBuffer_.end() || (*it)->width() != width ){
		return new Buffer(width, pHeight);
	}else{
		Buffer* const buf = *it;
		this->unusedBuffer_.erase(it);
		return buf;
	}
}

void Canvas::backBuffer(Buffer* buffer)
{
	auto ins = std::upper_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), buffer, internal::BufferOrder());
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

Canvas::ScissorScope::ScissorScope(Canvas& canvas, const geom::Area& area)
:canvas_(canvas)
{
	this->canvas_.scissor(area);
}

Canvas::ScissorScope::~ScissorScope()
{
	this->canvas_.scissorReset();
}

Canvas::AffineScope::AffineScope(Canvas& canvas)
:canvas_(canvas)
{
	glPushMatrix();
}
Canvas::AffineScope::~AffineScope()
{
	glPopMatrix();
}


}}

