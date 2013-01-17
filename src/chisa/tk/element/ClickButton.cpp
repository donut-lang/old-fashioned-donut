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

#include "ClickButton.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"

namespace chisa {
namespace tk {

static std::string const TAG("Button");

const std::string ClickButton::AttrName::ShadowColor("shadow-color");
const std::string ClickButton::AttrName::ShadowDepth("shadow-depth");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ClickButton, AbstractButton)
,shadowColor_(gl::DarkGray)
,shadowDepth_(3.0f)
{
	this->addAttribute(AttrName::ShadowColor, this->shadowColor_);
	this->addAttribute(AttrName::ShadowDepth, this->shadowDepth_);
}

ClickButton::~ClickButton() noexcept
{
}

std::string ClickButton::toString() const
{
	return ::tarte::format("(Button text:\"%s\" %p)", this->text().c_str(), this);
}

void ClickButton::shadowColor(gl::Color const& color)
{
	if(this->shadowColor_ != color){
		this->shadowColor_ = color;
	}
}
void ClickButton::shadowDepth(float const& depth)
{
	if( this->shadowDepth_ != depth ) {
		this->shadowDepth_ = depth;
	}
}

void ClickButton::renderOn(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	canvas.fillRect(this->shadowColor_, geom::Area(ptInRoot, geom::Box(innerSize().width(), shadowDepth_)) );
	canvas.fillRect(this->shadowColor_, geom::Area(ptInRoot, geom::Box(shadowDepth_, innerSize().height())) );

	geom::Distance const offset(this->renderOffset_+geom::Distance(shadowDepth_, shadowDepth_));
	geom::Area const buttonMask(mask.point()-offset, mask.box());
	this->textImage()->draw(canvas, ptInRoot+offset, buttonMask);
}
void ClickButton::renderOff(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	canvas.fillRect(this->shadowColor_, geom::Area(ptInRoot+geom::Distance(0, innerSize().height()-shadowDepth_), geom::Box(innerSize().width(), shadowDepth_)) );
	canvas.fillRect(this->shadowColor_, geom::Area(ptInRoot+geom::Distance(innerSize().width()-shadowDepth_, 0), geom::Box(shadowDepth_, innerSize().height())) );

	geom::Distance const offset(this->renderOffset_);
	geom::Area const buttonMask(mask.point()-offset, mask.box());
	this->textImage()->draw(canvas, ptInRoot+offset, buttonMask);
}
void ClickButton::renderDisabled(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	canvas.fillRect(this->shadowColor_, geom::Area(ptInRoot, innerSize()) );
	canvas.fillRect(this->disabledBackgroundColor(), geom::Area(ptInRoot+geom::Distance(shadowDepth_/2, shadowDepth_/2), innerSize()-geom::Box(shadowDepth_, shadowDepth_)) );

	geom::Distance const offset( (innerSize()-textImage()->size())/2 );
	geom::Area const buttonMask(mask.point()-offset, mask.box());
	this->textImage()->draw(canvas, ptInRoot+offset, buttonMask);
}
bool ClickButton::isOn() const noexcept
{
	return this->pushedCount() > 0;
}

geom::Box ClickButton::measureButtonContent(geom::Box const& constraint)
{
	return this->textImage()->size()+geom::Distance(shadowDepth_,shadowDepth_);
}

void ClickButton::layoutButtonContent(geom::Box const& size)
{
	this->renderOffset_ = ((size-this->textImage()->size()-geom::Distance(shadowDepth_,shadowDepth_))/2);
}

void ClickButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

}}
