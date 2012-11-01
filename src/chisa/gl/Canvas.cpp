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
,spriteManager_(log)
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
	sprite->drawImpl(this, pt, depth);
}
void Canvas::drawSprite(Handler<Sprite> sprite, const geom::Point& pt, const geom::Area& spriteArea, const float depth)
{
	sprite->drawImpl(this, pt, spriteArea, depth);
}
void Canvas::drawTexture(unsigned int texId, const geom::Point& pt, const geom::IntBox& texSize, const geom::Area& spriteArea, const float depth)
{
	const float width = spriteArea.width();
	const float height = spriteArea.height();

	const float top = spriteArea.y()/texSize.height();
	const float left = spriteArea.x()/texSize.width();
	const float right = (spriteArea.x()+width)/texSize.width();
	const float bottom = (spriteArea.y()+height)/texSize.height();
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBegin(GL_POLYGON);
		glTexCoord2f(left, top   );glVertex3f(pt.x()      , pt.y(), depth);
		glTexCoord2f(left, bottom);glVertex3f(pt.x()      , pt.y()+height, depth);
		glTexCoord2f(right,bottom);glVertex3f(pt.x()+width, pt.y()+height, depth);
		glTexCoord2f(right,top   );glVertex3f(pt.x()+width, pt.y(),depth);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFlush();
}
void Canvas::drawTexture(unsigned int texId, const geom::Point& pt, const geom::IntBox& texSize, const geom::IntBox& spriteSize, const float depth)
{
	this->drawTexture(texId, pt, texSize, geom::Area(geom::ZERO, spriteSize), depth);
}

Handler<Sprite> Canvas::queryRawSprite(const int width, const int height)
{
	return this->spriteManager_.queryRawSprite(width, height);
}

Handler<Drawable> Canvas::queryDrawable(const std::string& repl, const geom::Box& box)
{
	return this->drawableFactory_.queryDrawable(box, repl);
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

void Canvas::drawRect(const float width, const Color& color, const geom::Area& area, const float depth)
{
	glLineWidth(width);
	this->setColor(color);
	float const sx = area.x();
	float const ex = area.x()+area.width();
	float const sy = area.y();
	float const ey = area.y()+area.height();
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(sx, sy, depth);
		glVertex3f(ex, sy, depth);
		glVertex3f(ex, ey, depth);
		glVertex3f(sx, ey, depth);
	}
	glEnd();
	glFlush();
}
void Canvas::fillRect(const Color& color, const geom::Area& area, const float depth)
{
	this->setColor(color);
	float const sx = area.x();
	float const ex = area.x()+area.width();
	float const sy = area.y();
	float const ey = area.y()+area.height();
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(sx, sy, depth);
		glVertex3f(ex, sy, depth);
		glVertex3f(ex, ey, depth);
		glVertex3f(sx, ey, depth);
	}
	glEnd();
	glFlush();
}


void Canvas::setColor(const Color& color)
{
	glColor4f(color.red(), color.green(), color.blue(), color.alpha());
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

