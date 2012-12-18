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

#include "AbstractButton.h"
#include <tinyxml2.h>
#include "../../logging/Exception.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

#include "ElementGroup.h"

#include <iostream>
namespace chisa {
namespace tk {

const std::string AbstractButton::AttrName::Text("text");
const std::string AbstractButton::AttrName::TextSize("text-size");
const std::string AbstractButton::AttrName::EdgeColor("edge-color");
const std::string AbstractButton::AttrName::ShadowColor("shadow-color");
const std::string AbstractButton::AttrName::ShadowDepth("shadow-depth");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(AbstractButton, Element)
,text_()
,textSize_(32.0f)
,vertical_(false)
,edgeColor_(gl::Black)
,shadowColor_(gl::DarkGray)
,shadowDepth_(3.0f)
,textImage_()
,pushedCnt_(0)
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
	this->addAttribute(AttrName::EdgeColor, this->edgeColor_);
	this->addAttribute(AttrName::ShadowColor, this->shadowColor_);
	this->addAttribute(AttrName::ShadowDepth, this->shadowDepth_);
}

AbstractButton::~AbstractButton() noexcept
{
}

std::string AbstractButton::toString() const
{
	return util::format("(AbstractButton text:\"%s\" %p)", this->text_.c_str(), this);
}

void AbstractButton::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( this->pushedCnt_ > 0 ){
		canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point(), geom::Box(screenArea.width(), shadowDepth_)),.001);
		canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point(), geom::Box(shadowDepth_, screenArea.height())),.001);
		this->textImage()->draw(canvas, geom::Area(screenArea.point()+this->renderOffset_+geom::Distance(shadowDepth_, shadowDepth_), area.box()), .001);
	}else{
		canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point()+geom::Distance(0, screenArea.height()-shadowDepth_), geom::Box(screenArea.width(), shadowDepth_)),.001);
		canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point()+geom::Distance(screenArea.width()-shadowDepth_, 0), geom::Box(shadowDepth_, screenArea.height())),.001);
		this->textImage()->draw(canvas, geom::Area(screenArea.point()+this->renderOffset_, area.box()), .001);
	}
}
Handler<gl::TextDrawable> AbstractButton::textImage()
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
						this->foregroundColor(),
						gl::Transparent
						);
			}else{
				this->textImage_ = w->drawableManager()->queryText(
						this->text(),
						this->textSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						this->foregroundColor(),
						gl::Transparent
						);
			}
		}
	}
	return this->textImage_;
}
geom::Box AbstractButton::measureImpl(geom::Box const& constraint)
{
	return this->textImage()->size()+geom::Distance(shadowDepth_,shadowDepth_);
}

void AbstractButton::layoutImpl(geom::Box const& size)
{
	//中心になるようにオフセットを設定する。
	this->renderOffset_ = ((size-this->textImage_->size()-geom::Distance(shadowDepth_,shadowDepth_))/2);
}

void AbstractButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void AbstractButton::text(std::string const& text)
{
	if(this->text_ != text){
		this->text_ = text;
		this->textImage_.reset();
		this->invalidate();
	}
}

void AbstractButton::textSize(float const& size)
{
	if( this->textSize_ != size ) {
		this->textSize_ = size;
		this->textImage_.reset();
		this->invalidate();
	}
}
void AbstractButton::setVertical(bool isVertical)
{
	if( this->vertical_ != isVertical ) {
		this->vertical_ = isVertical;
		this->textImage_.reset();
		this->invalidate();
	}
}
void AbstractButton::shadowColor(gl::Color const& color)
{
	if(this->shadowColor_ != color){
		this->shadowColor_ = color;
		this->invalidate();
	}
}
void AbstractButton::shadowDepth(float const& depth)
{
	if( this->shadowDepth_ != depth ) {
		this->shadowDepth_ = depth;
		this->invalidate();
	}
}

void AbstractButton::onClick()
{
	std::cout << "CLICK" << std::endl;
	Handler<ElementGroup> h(this->findRootElement()->findElementById("frame").cast<ElementGroup>());
	h->bringChildToLast(h->getChildAt(0));
	h->invalidate();
}

bool AbstractButton::onDownRaw(const float timeMs, geom::Point const& ptInScreen)
{
	++this->pushedCnt_;
	return false;
}

bool AbstractButton::onUpRaw(const float timeMs, geom::Point const& ptInScreen)
{
	--this->pushedCnt_;
	return false;
}

bool AbstractButton::onMoveRaw(const float timeMs, geom::Point const& ptInScreen)
{
	return true;
}

bool AbstractButton::onSingleTapUp(const float timeMs, geom::Point const& ptInScreen)
{
	if(this->screenArea().contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}
