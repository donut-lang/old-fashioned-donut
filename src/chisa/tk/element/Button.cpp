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

#include <iostream>
namespace chisa {
namespace tk {
namespace element {

const std::string Button::AttrName::Text("text");
const std::string Button::AttrName::TextSize("text-size");
const std::string Button::AttrName::ForegroundColor("foreground-color");
const std::string Button::AttrName::BackgroundColor("background-color");
const std::string Button::AttrName::ShadowColor("shadow-color");
const std::string Button::AttrName::ShadowDepth("shadow-depth");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Button, LeafElement)
,text_()
,textSize_(32.0f)
,margin_(2.5f)
,padding_(2.0f)
,vertical_(false)
,foregroundColor_(gl::Black)
,backgroundColor_(gl::Color(0.9,0.9,0.9,1))
,shadowColor_(gl::DarkGray)
,shadowDepth_(3.0f)
,textImage_()
,pushedCnt_(0)
{
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
}

Button::~Button() noexcept
{
}

std::string Button::toString() const
{
	return util::format("(Button text:\"%s\" %p)", this->text_.c_str(), this);
}

void Button::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	geom::Area marginedArea(this->margin_.apply(screenArea));
	canvas.fillRect(this->backgroundColor_, marginedArea);
	if( this->pushedCnt_ > 0 ){
		canvas.fillRect(this->shadowColor_, geom::Area(marginedArea.point(), geom::Box(marginedArea.width(), shadowDepth_)),.001);
		canvas.fillRect(this->shadowColor_, geom::Area(marginedArea.point(), geom::Box(shadowDepth_, marginedArea.height())),.001);
		this->textImage()->draw(canvas, geom::Area(marginedArea.point()+this->renderOffset_+geom::Distance(shadowDepth_, shadowDepth_), area.box()), .001);
	}else{
		canvas.fillRect(this->shadowColor_, geom::Area(marginedArea.point()+geom::Distance(0, marginedArea.height()-shadowDepth_), geom::Box(marginedArea.width(), shadowDepth_)),.001);
		canvas.fillRect(this->shadowColor_, geom::Area(marginedArea.point()+geom::Distance(marginedArea.width()-shadowDepth_, 0), geom::Box(shadowDepth_, marginedArea.height())),.001);
		this->textImage()->draw(canvas, geom::Area(marginedArea.point()+this->renderOffset_, area.box()), .001);
	}
}
Handler<gl::TextDrawable> Button::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock()){
			if(vertical_){
				this->textImage_ = w->drawableManager()->queryVerticalText(
						this->text(),
						this->textSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						this->foregroundColor_,
						gl::Transparent
						);
			}else{
				this->textImage_ = w->drawableManager()->queryText(
						this->text(),
						this->textSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						this->foregroundColor_,
						gl::Transparent
						);
			}
		}
	}
	return this->textImage_;
}
geom::Box Button::onMeasure(geom::Box const& constraint)
{
	return this->textImage()->size()+this->padding_.totalSpace()+this->margin_.totalSpace()+geom::Distance(shadowDepth_,shadowDepth_);
}

void Button::onLayout(geom::Box const& size)
{
	//中心になるようにオフセットを設定する。
	this->renderOffset_ = ((size-this->margin_.totalSpace()-this->padding_.totalSpace()-this->textImage_->size()-geom::Distance(shadowDepth_,shadowDepth_))/2)+this->padding_.offset();
}

void Button::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void Button::text(std::string const& text)
{
	this->text_ = text;
	this->textImage_.reset();
	//TODO: レイアウトの再設定が必要
}

void Button::onClick()
{
	std::cout << "CLICK" << std::endl;
}

bool Button::onDownRaw(const float timeMs, geom::Point const& ptInScreen)
{
	++this->pushedCnt_;
	return false;
}

bool Button::onUpRaw(const float timeMs, geom::Point const& ptInScreen)
{
	--this->pushedCnt_;
	return false;
}

bool Button::onMoveRaw(const float timeMs, geom::Point const& ptInScreen)
{
	return true;
}

bool Button::onSingleTapUp(const float timeMs, geom::Point const& ptInScreen)
{
	if(this->screenArea().contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}}
