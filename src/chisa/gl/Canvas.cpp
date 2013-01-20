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
#include "OpenGL.h"
#include <algorithm>

namespace chisa {
namespace gl {

const static std::string TAG("Canvas");

Canvas::Canvas(Logger& log)
:log_(log)
,width_(NAN)
,height_(NAN)
,glOperation_(None)
,vertexs_()
,texCoords_()
,nowColor_()
,nowLineWidth_(-1)
,nowScissor_(nullptr)
,vertexArrayEnabled_(false)
,textureEnabled_(false)
,nowTexId_(0)
{

}

Canvas::~Canvas()
{

}

void Canvas::ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val)
{
#if IS_GLES
	glOrthof(left, right, bottom, top, near_val, far_val);
#else
	glOrtho(left, right, bottom, top, near_val, far_val);
#endif
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
	this->bindTexture(texId);
	this->setColor(color);
	this->setOperation(Texture);
	pushTexCoord(left ,top   );pushVertex(x      , y       , depth);
	pushTexCoord(left ,bottom);pushVertex(x      , y+height, depth);
	pushTexCoord(right,top   );pushVertex(x+width, y       , depth);

	pushTexCoord(right,top   );pushVertex(x+width, y       , depth);
	pushTexCoord(left ,bottom);pushVertex(x      , y+height, depth);
	pushTexCoord(right,bottom);pushVertex(x+width, y+height, depth);
	flushGL();
}

void Canvas::drawLine(const float width, Color const& color, geom::Point const& start, geom::Point const& end, const float depth)
{
	if(color.isInvalid() || !(width > 0)){
		return;
	}
	this->setLineWidth(width);
	this->setColor(color);
	this->setOperation(Lines);
	pushVertex(start.x(), start.y(), depth);
	pushVertex(end.x()  , end.y(), depth);
}

void Canvas::drawLines(const float width, Color const& color, std::vector<geom::Point> const& pts, const float depth)
{
	if(color.isInvalid() || !(width > 0)){
		return;
	}
	this->setLineWidth(width);
	this->setColor(color);
	this->setOperation(LineStrip);
	for(geom::Point const& pt : pts){
		pushVertex(pt.x(), pt.y(), depth);
	}
	flushGL();
}
void Canvas::drawRect(const float width, Color const& color, geom::Area const& area, const float depth)
{
	if(color.isInvalid() || !(width > 0)){
		return;
	}
	float const sx = area.x();
	float const ex = area.x()+area.width();
	float const sy = area.y();
	float const ey = area.y()+area.height();
	this->setColor(color);
	this->setLineWidth(width);
	this->setOperation(Lines);
	pushVertex(sx, sy, depth);
	pushVertex(ex, sy, depth);

	pushVertex(ex, sy, depth);
	pushVertex(ex, ey, depth);

	pushVertex(ex, ey, depth);
	pushVertex(sx, ey, depth);

	pushVertex(sx, ey, depth);
	pushVertex(sx, sy, depth);
}
void Canvas::fillRect(Color const& color, geom::Area const& area, const float depth)
{
	if(color.isInvalid() || area.empty()){
		return;
	}
	float const sx = area.x();
	float const ex = area.x()+area.width();
	float const sy = area.y();
	float const ey = area.y()+area.height();
	this->setColor(color);
	this->setOperation(Rect);
	pushVertex(sx, sy, depth);
	pushVertex(sx, ey, depth);
	pushVertex(ex, sy, depth);

	pushVertex(ex, sy, depth);
	pushVertex(sx, ey, depth);
	pushVertex(ex, ey, depth);
	flushGL();
}

void Canvas::flushGL()
{
	switch( this->glOperation_ ) {
	case None:
		break;
	case Lines: {
		enableVertexArray();
		disableTexture();
		glVertexPointer(3, GL_FLOAT, 0, this->vertexs_.data());
		glDrawArrays(GL_LINES, 0, this->vertexs_.size()/3);
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to draw arrays: 0x%08x", err);
			}
		}
#endif
		break;
	}
	case LineStrip: {
		enableVertexArray();
		disableTexture();
		glVertexPointer(3, GL_FLOAT, 0, this->vertexs_.data());
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to set vertex pointer: 0x%08x", err);
			}
		}
#endif
		glDrawArrays(GL_LINE_STRIP, 0, this->vertexs_.size()/3);
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to draw arrays: 0x%08x", err);
			}
		}
#endif
		break;
	}
	case Texture: {
		enableVertexArray();
		enableTexture();
		glVertexPointer(3, GL_FLOAT, 0, this->vertexs_.data());
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to set vertex pointer: 0x%08x", err);
			}
		}
#endif
		glTexCoordPointer(2, GL_FLOAT, 0, this->texCoords_.data());
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to set vertex pointer: 0x%08x", err);
			}
		}
#endif
		glDrawArrays(GL_TRIANGLES, 0, this->vertexs_.size()/3);
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to draw arrays: 0x%08x", err);
			}
		}
#endif
		break;
	}
	case Rect: {
		enableVertexArray();
		disableTexture();
		glVertexPointer(3, GL_FLOAT, 0, this->vertexs_.data());
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to set vertex pointer: 0x%08x", err);
			}
		}
#endif
		glDrawArrays(GL_TRIANGLES, 0, this->vertexs_.size()/3);
#ifdef DEBUG
		{
			const GLenum err = glGetError();
			if(err != GL_NO_ERROR){
				TARTE_EXCEPTION(Exception, "[BUG] Failed to draw arrays: 0x%08x", err);
			}
		}
#endif
		break;
	}
	default:
		TARTE_EXCEPTION(Exception, "Unknown Operation", this->glOperation_);
		break;
	}
	vertexs_.clear();
	texCoords_.clear();
	this->glOperation_ = None;
}

void Canvas::disableVertexArray()
{
	if(vertexArrayEnabled_) {
		glDisableClientState(GL_VERTEX_ARRAY);
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to disable vertex array: 0x%08x", err);
	}
#endif
		vertexArrayEnabled_ = false;
	}
}

void Canvas::flush()
{
	flushGL();
}

void Canvas::enableVertexArray()
{
	if(!vertexArrayEnabled_) {
		glEnableClientState(GL_VERTEX_ARRAY);
#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to enable vertex array: 0x%08x", err);
		}
#endif
		vertexArrayEnabled_ = true;
	}
}
void Canvas::disableTexture()
{
	if(textureEnabled_){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to disable texture: 0x%08x", err);
		}
#endif
		textureEnabled_=false;
	}
}
void Canvas::enableTexture()
{
	if(!textureEnabled_){
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to enable texture: 0x%08x", err);
		}
#endif
		textureEnabled_=true;
	}
}

void Canvas::setColor(Color const& color)
{
	if( color != nowColor_ ){
		flushGL();
		this->nowColor_ = color;
		glColor4f(nowColor_.red(), nowColor_.green(), nowColor_.blue(), nowColor_.alpha());
	#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to flush color: 0x%08x", err);
		}
	#endif
	}
}
void Canvas::setOperation(GLOperation const& op)
{
	if( op != glOperation_ ){
		flushGL();
		this->glOperation_ = op;
	}
}
void Canvas::setLineWidth( float const& lineWidth )
{

	if( std::fabs(nowLineWidth_-lineWidth) >= 1.0f ) {
		flushGL();
		this->nowLineWidth_ = lineWidth;
		glLineWidth(nowLineWidth_);
	#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to flush line width: 0x%08x", err);
		}
	#endif
	}
}

void Canvas::bindTexture(GLuint texId)
{
	if(nowTexId_ != texId){
		flushGL();
		glBindTexture(GL_TEXTURE_2D, this->nowTexId_ = texId);
	#ifdef DEBUG
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to flush texture: 0x%08x", err);
		}
	#endif
	}
}

//---------------------------------------------------------------------------------------

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
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to push martix: 0x%08x", err);
	}
#endif
}
Canvas::AffineScope::~AffineScope()
{
	glPopMatrix();
#ifdef DEBUG
	const GLenum err = glGetError();
	if(err != GL_NO_ERROR){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to pop matrix: 0x%08x", err);
	}
#endif
}


}}

