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

#include <tarte/Exception.h>

#include "AbstractToggleButton.h"
#include "../../gl/Canvas.h"

namespace chisa {
namespace tk {

const std::string AbstractToggleButton::AttrName::OnForegroundColor("foreground-color-on");
const std::string AbstractToggleButton::AttrName::OnBackgroundColor("background-color-on");
const std::string AbstractToggleButton::AttrName::OffForegroundColor("foreground-color-off");
const std::string AbstractToggleButton::AttrName::OffBackgroundColor("background-color-off");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(AbstractToggleButton, AbstractButton)
,onForegroundColor_(foregroundColor())
,onBackgroundColor_(backgroundColor())
,offForegroundColor_(foregroundColor().darker())
,offBackgroundColor_(backgroundColor().darker())
{
	this->addAttribute(AttrName::OnForegroundColor, this->onForegroundColor_);
	this->addAttribute(AttrName::OnBackgroundColor, this->onBackgroundColor_);
	this->addAttribute(AttrName::OffForegroundColor, this->offForegroundColor_);
	this->addAttribute(AttrName::OffBackgroundColor, this->offBackgroundColor_);
}

AbstractToggleButton::~AbstractToggleButton() noexcept
{
}

std::string AbstractToggleButton::toString() const
{
	return ::tarte::format("(AbstractToggleButton text:\"%s\" %p)", this->text().c_str(), this);
}

void AbstractToggleButton::checked(bool const& state)
{
	this->checkedImpl(state);
	this->notifyCheckedStateChanged();
}

void AbstractToggleButton::notifyCheckedStateChanged()
{
	if(this->checked()){
		this->backgroundColor(this->onBackgroundColor());
		this->foregroundColor(this->onForegroundColor());
	}else{
		this->backgroundColor(this->offBackgroundColor());
		this->foregroundColor(this->offForegroundColor());
	}
}

void AbstractToggleButton::onBackgroundColor(gl::Color const& c)
{
	this->onBackgroundColor_ = c;
	if(this->checked()) {
		this->backgroundColor(c);
	}
}
void AbstractToggleButton::onForegroundColor(gl::Color const& c)
{
	this->onForegroundColor_ = c;
	if(this->checked()) {
		this->foregroundColor(c);
	}
}
void AbstractToggleButton::offBackgroundColor(gl::Color const& c)
{
	this->offBackgroundColor_ = c;
	if(!this->checked()) {
		this->backgroundColor(c);
	}
}
void AbstractToggleButton::offForegroundColor(gl::Color const& c)
{
	this->offForegroundColor_ = c;
	if(!this->checked()) {
		this->foregroundColor(c);
	}
}

bool AbstractToggleButton::checked() const
{
	return this->checkedImpl();
}

void AbstractToggleButton::toggle()
{
	this->checked(!this->checked());
}

gl::Color AbstractToggleButton::onBackgroundColor() const
{
	return this->onBackgroundColor_;
}
gl::Color AbstractToggleButton::onForegroundColor() const
{
	return this->onForegroundColor_;
}
gl::Color AbstractToggleButton::offBackgroundColor() const
{
	return this->offBackgroundColor_;
}
gl::Color AbstractToggleButton::offForegroundColor() const
{
	return this->offForegroundColor_;
}

void AbstractToggleButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	this->AbstractButton::loadXmlImpl(factory, element);
}

void AbstractToggleButton::onClick()
{
	this->toggle();
}

bool AbstractToggleButton::isOn() const noexcept
{
	return this->checked();
}

geom::Box AbstractToggleButton::measureButtonContent(geom::Box const& constraint)
{
	return this->textImage()->size();
}
void AbstractToggleButton::layoutButtonContent(geom::Box const& size)
{
	this->renderOffset_ = ((size-this->textImage()->size())/2);
}
void AbstractToggleButton::renderOn(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	this->textImage()->draw(canvas, ptInRoot, mask);
}
void AbstractToggleButton::renderOff(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	this->textImage()->draw(canvas, ptInRoot, mask);
}


}}
