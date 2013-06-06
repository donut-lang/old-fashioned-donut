/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Exception.h>

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
	return ::cinamo::format("(AbstractToggleButton text:\"%s\" %p)", this->text().c_str(), this);
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
void AbstractToggleButton::renderDisabled(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	///XXX: 実装
	this->textImage()->draw(canvas, ptInRoot, mask);
}


}}
