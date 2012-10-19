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

#include "Button.h"
#include <cairo/cairo.h>
#include <tinyxml2.h>
#include "../../gl/Canvas.h"
#include "../../logging/Exception.h"

namespace chisa {
namespace tk {
namespace widget {

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(Button)
,surface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1,1))
,cairo_(cairo_create(this->surface_))
,text_()
,sprite_()
,dirty_(false)
,vertical_(false)
,pushed_(false)
{
	{
		cairo_status_t st = cairo_surface_status(this->surface_);
		if(st != CAIRO_STATUS_SUCCESS){
			throw logging::Exception(__FILE__, __LINE__, "Oops. Failed to create cairo surface. Error code: %d", st);
		}
	}
	{
		cairo_status_t st = cairo_status(this->cairo_);
		if(st != CAIRO_STATUS_SUCCESS){
			throw logging::Exception(__FILE__, __LINE__, "Oops. Failed to create cairo. Error code: %d", st);
		}
	}
}

Button::~Button()
{
	cairo_destroy(this->cairo_);
	cairo_surface_destroy(this->surface_);
}

weak_ptr<Layout> Button::getChildAt(const size_t index) const
{
	return weak_ptr<Layout>();
}

size_t Button::getChildCount() const
{
	return 0;
}

string Button::toString() const
{
	return util::format("(Button text:\"%s\")", this->text_.c_str());
}

void Button::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	canvas.drawSprite(this->sprite_, this->renderOffset_, 0.0f);
}

geom::Box Button::onMeasure(const geom::Box& constraint)
{
	this->measureTextSize();
	return this->textSize_;
}

void Button::onLayout(const geom::Box& size)
{
	this->renderOffset_ = (size-this->textSize_)/2;
}

void Button::loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element)
{
	if(const char* text = element->Attribute("text")){
		this->text(text);
	}
}

void Button::text(const std::string& text)
{
	this->text_ = text;
	this->dirty_ = true;
}

void Button::realizeText(gl::Canvas& canvas)
{
	if(!this->dirty_){
		return;
	}
	this->dirty_ = false;
	this->measureTextSize();
	this->sprite_ = canvas.queryRawSprite(static_cast<int>(this->textSize_.width()), static_cast<int>(this->textSize_.height()));
	{
		gl::RawSprite::Session ss(this->sprite_);
		cairo_surface_t* surf = cairo_image_surface_create_for_data(ss.data(), CAIRO_FORMAT_ARGB32, ss.width(), ss.height(), ss.stride());
		cairo_t* cr = cairo_create(surf);

		//データは使いまわしているので一旦サーフェイスの中身を削除する
		cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
		cairo_paint(cr);

		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
		cairo_move_to(cr, this->textOffset_.x(), this->textOffset_.y());
		if(this->vertical_){
			cairo_rotate(cr, 90);
		}

		cairo_text_path(cr, this->text_.c_str());
		cairo_set_source_rgba(cr, 1,1,1,1);
		cairo_paint(cr);

		cairo_destroy(cr);
	}
}

void Button::measureTextSize()
{
	cairo_text_extents_t ext;
	cairo_text_extents(this->cairo_, this->text_.c_str(), &ext);
	this->textSize_ = vertical_ ? geom::Box(ext.height+ext.y_advance, ext.x_advance) : geom::Box(ext.x_advance, ext.height+ext.y_advance);
	this->textOffset_ = vertical_ ? geom::Vector(-ext.y_bearing, ext.x_bearing) : geom::Vector(ext.x_bearing, -ext.y_bearing);
}

void Button::onClick()
{

}

weak_ptr<Layout> Button::getLayoutByIdImpl(const std::string& id)
{
	return weak_ptr<Layout>();
}

bool Button::onDownRaw(const float timeMs, const geom::Point& ptInScreen)
{
	this->pushed_ = true;
	return false;
}

bool Button::onUpRaw(const float timeMs, const geom::Point& ptInScreen)
{
	this->pushed_ = false;
	return false;
}


bool Button::onSingleTapUp(const float timeMs, const geom::Point& ptInScreen)
{
	geom::Area a(this->drawnArea().point()+this->renderOffset_, this->textSize_);
	if(a.contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}}
