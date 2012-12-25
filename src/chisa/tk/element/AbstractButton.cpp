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

#include <tinyxml2.h>
#include <tarte/Exception.h>

#include "AbstractButton.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

#include "ElementGroup.h"
namespace chisa {
namespace tk {

const std::string AbstractButton::AttrName::Text("text");
const std::string AbstractButton::AttrName::TextSize("text-size");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(AbstractButton, Element)
,text_()
,textSize_(32.0f)
,vertical_(false)
,textImage_()
,pushedCnt_(0)
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->edgeWidth(1);
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
}

AbstractButton::~AbstractButton() noexcept
{
}

std::string AbstractButton::toString() const
{
	return ::tarte::format("(AbstractButton text:\"%s\" %p)", this->text_.c_str(), this);
}

void AbstractButton::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if(this->isOn()) {
		this->renderOn(canvas, screenArea, area);
	}else{
		this->renderOff(canvas, screenArea, area);
	}
}
Handler<gl::TextDrawable> AbstractButton::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock() ){
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
	return this->measureButtonContent(constraint);
}

void AbstractButton::layoutImpl(geom::Box const& size)
{
	return this->layoutButtonContent(size);
}

void AbstractButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void AbstractButton::text(std::string const& text)
{
	if(this->text_ != text){
		this->text_ = text;
		this->notifyViewRefreshed();
	}
}

void AbstractButton::textSize(float const& size)
{
	if( this->textSize_ != size ) {
		this->textSize_ = size;
		this->notifyViewRefreshed();
	}
}
void AbstractButton::setVertical(bool isVertical)
{
	if( this->vertical_ != isVertical ) {
		this->vertical_ = isVertical;
		this->notifyViewRefreshed();
	}
}

void AbstractButton::onClick()
{
}

bool AbstractButton::notifyViewRefreshedImpl()
{
	this->textImage_.reset();
	return true;
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
