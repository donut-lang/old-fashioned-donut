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

namespace chisa {
namespace gl {

const static std::string TAG("Canvas");

Canvas::Canvas(Logger& log)
:log_(log)
,width_(NAN)
,height_(NAN)
,nowScissor_(nullptr)
{

}

Canvas::~Canvas()
{

}

void Canvas::ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val)
{
	glOrtho(left, right, bottom, top, near_val, far_val);
	const GLenum err = glGetError();
#ifdef DEBUG
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec glOrtho: 0x%08x", err);
	}
#endif
}
void Canvas::translate(geom::Point const& pt)
{
	glTranslatef(pt.x(),pt.y(),0.0f);
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec glTranslatef: 0x%08x", err);
	}
#endif
}
void Canvas::rotate(const float angle)
{
	glRotatef(angle, 0, 0, 1.0f);
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec glRotetef: 0x%08x", err);
	}
#endif
}
void Canvas::scale(geom::ScaleVector const& scale)
{
	glScalef(scale.x(),scale.y(),1.0f);
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec glScalef: 0x%08x", err);
	}
#endif
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
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec resize2d: 0x%08x", err);
	}
#endif
}

void Canvas::scissor(geom::Area const& area)
{
	glScissor(area.x(), this->height_-area.height()-area.y(),area.width(), area.height());
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec glScissor: 0x%08x", err);
	}
#endif
}

void Canvas::drawSprite(Handler<Sprite> const& sprite, geom::Point const& ptInRoot, const float depth, Color const& color)
{
	sprite->drawImpl(this, ptInRoot, depth, color);
}
void Canvas::drawSprite(Handler<Sprite> const& sprite, geom::Point const& ptInRoot, geom::Area const& mask, const float depth, Color const& color)
{
	sprite->drawImpl(this, ptInRoot, mask, depth, color);
}
void Canvas::drawTexture(unsigned int texId, geom::Area const& areaInRoot, geom::Area const& coordinateInSprite, const float depth, Color const& color)
{
	const float x=areaInRoot.x();
	const float y=areaInRoot.y();
	const float width = areaInRoot.width();
	const float height = areaInRoot.height();

	const float top = coordinateInSprite.y();
	const float left = coordinateInSprite.x();
	const float right = coordinateInSprite.x()+coordinateInSprite.width();
	const float bottom = coordinateInSprite.y()+coordinateInSprite.height();
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnable(GL_TEXTURE_2D);
	this->setColor(color);
	glBegin(GL_POLYGON);
		glTexCoord2f(left ,top   );glVertex3f(x      , y       , depth);
		glTexCoord2f(left ,bottom);glVertex3f(x      , y+height, depth);
		glTexCoord2f(right,bottom);glVertex3f(x+width, y+height, depth);
		glTexCoord2f(right,top   );glVertex3f(x+width, y       , depth);
	glEnd();
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec drawTexture: 0x%08x", err);
	}
#endif
	glDisable(GL_TEXTURE_2D);
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
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec drawLine: 0x%08x", err);
	}
#endif
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
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec drawRect: 0x%08x", err);
	}
#endif
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
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to exec fillRect: 0x%08x", err);
	}
#endif
}


void Canvas::setColor(Color const& color)
{
	glColor4f(color.red(), color.green(), color.blue(), color.alpha());
}

Canvas::ScissorScope::ScissorScope(Canvas& canvas, geom::Area const& area)
:canvas_(canvas)
,area_(area)
,orig_(canvas.nowScissor_)
{
	this->canvas_.nowScissor_ = this;
	this->canvas_.scissor(area);
	if(!this->orig_){
		glEnable(GL_SCISSOR_TEST);
	}
}

Canvas::ScissorScope::~ScissorScope()
{
	this->canvas_.nowScissor_ = orig_;
	if(orig_){
		this->canvas_.scissor(orig_->area_);
	}else{
		glDisable(GL_SCISSOR_TEST);
	}
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

