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
#include <tinyxml2.h>
#include "../../gl/Canvas.h"
#include "../../logging/Exception.h"

namespace chisa {
namespace tk {
namespace widget {

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(Button)
,text_()
,sprite_()
,dirty_(false)
,vertical_(false)
,pushedCnt_(0)
{
}

Button::~Button()
{
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
	this->renderer_.renderString(this->sprite_, this->cmd_, this->vertical_ ? 90.0f : 0.0f);
}

void Button::measureTextSize()
{
	this->cmd_ = this->renderer_.measure(this->text().c_str());
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
	++this->pushedCnt_;
	return false;
}

bool Button::onUpRaw(const float timeMs, const geom::Point& ptInScreen)
{
	--this->pushedCnt_;
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
