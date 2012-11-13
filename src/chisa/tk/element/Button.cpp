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
#include "../World.h"
#include "../../logging/Exception.h"
#include "../../gl/DrawableManager.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(Button)
,text_()
,textImage_()
,vertical_(false)
,pushedCnt_(0)
{
}

Button::~Button() noexcept
{
}

weak_ptr<Element> Button::getChildAt(const size_t index) const
{
	return weak_ptr<Element>();
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
	this->textImage_->draw(canvas, geom::Area(screenArea.point()+this->renderOffset_, area.box()));
}
Handler<gl::TextDrawable> Button::textImage()
{
	if(!this->textImage_){
		if( shared_ptr<World> w = this->world().lock()){
			//TODO: 色とか
			if(vertical_){
				this->textImage_ = w->drawableManager()->queryVerticalText(this->text());
			}else{
				this->textImage_ = w->drawableManager()->queryText(this->text());
			}
		}
	}
	return this->textImage_;
}
geom::Box Button::onMeasure(const geom::Box& constraint)
{
	return this->textImage()->size();
}

void Button::onLayout(const geom::Box& size)
{
	this->renderOffset_ = (size-this->textImage_->size())/2;
}

void Button::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	if(const char* text = element->Attribute("text")){
		this->text(text);
	}
}

void Button::text(const std::string& text)
{
	this->text_ = text;
	this->textImage_.reset();
}

void Button::onClick()
{

}

weak_ptr<Element> Button::getElementByIdImpl(const std::string& id)
{
	return weak_ptr<Element>();
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
	geom::Area a(this->drawnArea().point()+this->renderOffset_, this->textImage()->size());
	if(a.contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}}
