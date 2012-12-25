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

Canvas::Canvas(Logger& log)
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
void Canvas::translate(geom::Point const& pt)
{
	glTranslatef(pt.x(),pt.y(),0.0f);
}
void Canvas::rotate(const float angle, geom::Point const& pt)
{
	glRotatef(angle, pt.x(),pt.y(),0.0f);
}
void Canvas::scale(geom::ScaleVector const& scale)
{
	glScalef(scale.x(),scale.y(),1.0f);
}

void Canvas::resize2d(geom::Box const& box)
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

void Canvas::scissor(geom::Area const& area)
{
	glScissor(area.x(), this->height_-area.height()-area.y(),area.width(), area.height());
}

void Canvas::scissorReset()
{
	glScissor(0,0,this->width_,this->height_);
}

void Canvas::drawSprite(Handler<Sprite> sprite, geom::Point const& pt, const float depth, Color const& color)
{
	sprite->drawImpl(this, pt, color, depth);
}
void Canvas::drawSprite(Handler<Sprite> sprite, geom::Point const& pt, geom::Area const& spriteArea, const float depth, Color const& color)
{
	sprite->drawImpl(this, pt, spriteArea, color, depth);
}
void Canvas::drawTexture(unsigned int texId, geom::Point const& pt, geom::IntBox const& texSize, geom::Area const& spriteArea, const float depth, Color const& color)
{
	const float width = spriteArea.width();
	const float height = spriteArea.height();

	const float top = spriteArea.y()/texSize.height();
	const float left = spriteArea.x()/texSize.width();
	const float right = (spriteArea.x()+width)/texSize.width();
	const float bottom = (spriteArea.y()+height)/texSize.height();
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnable(GL_TEXTURE_2D);
	this->setColor(color);
	glBegin(GL_POLYGON);
		glTexCoord2f(left, top   );glVertex3f(pt.x()      , pt.y(), depth);
		glTexCoord2f(left, bottom);glVertex3f(pt.x()      , pt.y()+height, depth);
		glTexCoord2f(right,bottom);glVertex3f(pt.x()+width, pt.y()+height, depth);
		glTexCoord2f(right,top   );glVertex3f(pt.x()+width, pt.y(),depth);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void Canvas::drawTexture(unsigned int texId, geom::Point const& pt, geom::IntBox const& texSize, geom::IntBox const& spriteSize, const float depth, Color const& color)
{
	this->drawTexture(texId, pt, texSize, geom::Area(geom::ZERO, spriteSize), depth, color);
}

void Canvas::drawLine(const float width, Color const& color, geom::Point const& start, geom::Point const& end, const float depth)
{
	if(color.isInvalid() || !(width > 0)){
		return;
	}
	glLineWidth(width);
	this->setColor(color);
	glBegin(GL_LINES);
		glVertex3f(start.x(), start.y(), depth);
		glVertex3f(end.x()  , end.y(), depth);
	glEnd();
}

void Canvas::drawRect(const float width, Color const& color, geom::Area const& area, const float depth)
{
	if(color.isInvalid() || !(width > 0)){
		return;
	}
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
}
void Canvas::fillRect(Color const& color, geom::Area const& area, const float depth)
{
	if(color.isInvalid() || area.empty()){
		return;
	}
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
}


void Canvas::setColor(Color const& color)
{
	glColor4f(color.red(), color.green(), color.blue(), color.alpha());
}

Canvas::ScissorScope::ScissorScope(Canvas& canvas, geom::Area const& area)
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

