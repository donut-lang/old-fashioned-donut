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

#include "Button.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

namespace chisa {
namespace tk {

const std::string Button::AttrName::ShadowColor("shadow-color");
const std::string Button::AttrName::ShadowDepth("shadow-depth");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Button, AbstractButton)
,shadowColor_(gl::DarkGray)
,shadowDepth_(3.0f)
{
	this->addAttribute(AttrName::ShadowColor, this->shadowColor_);
	this->addAttribute(AttrName::ShadowDepth, this->shadowDepth_);
}

Button::~Button() noexcept
{
}

std::string Button::toString() const
{
	return ::tarte::format("(Button text:\"%s\" %p)", this->text().c_str(), this);
}

void Button::shadowColor(gl::Color const& color)
{
	if(this->shadowColor_ != color){
		this->shadowColor_ = color;
	}
}
void Button::shadowDepth(float const& depth)
{
	if( this->shadowDepth_ != depth ) {
		this->shadowDepth_ = depth;
	}
}

void Button::renderOn(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point(), geom::Box(screenArea.width(), shadowDepth_)),.001);
	canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point(), geom::Box(shadowDepth_, screenArea.height())),.001);
	this->textImage()->draw(canvas, geom::Area(screenArea.point()+this->renderOffset_+geom::Distance(shadowDepth_, shadowDepth_), area.box()), .001);
}
void Button::renderOff(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point()+geom::Distance(0, screenArea.height()-shadowDepth_), geom::Box(screenArea.width(), shadowDepth_)),.001);
	canvas.fillRect(this->shadowColor_, geom::Area(screenArea.point()+geom::Distance(screenArea.width()-shadowDepth_, 0), geom::Box(shadowDepth_, screenArea.height())),.001);
	this->textImage()->draw(canvas, geom::Area(screenArea.point()+this->renderOffset_, area.box()), .001);
}
bool Button::isOn() const noexcept
{
	return this->pushedCount() > 0;
}

geom::Box Button::measureButtonContent(geom::Box const& constraint)
{
	return this->textImage()->size()+geom::Distance(shadowDepth_,shadowDepth_);
}

void Button::layoutButtonContent(geom::Box const& size)
{
	this->renderOffset_ = ((size-this->textImage()->size()-geom::Distance(shadowDepth_,shadowDepth_))/2);
}


}}
