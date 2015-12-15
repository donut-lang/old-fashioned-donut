/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <tinyxml2.h>
#include <cinamo/Exception.h>

#include "ClockSlider.hpp"
#include "../../gl/Canvas.hpp"
#include "../../gl/DrawableManager.hpp"
#include "../World.hpp"

#include "ElementGroup.hpp"
namespace chisa {
namespace tk {

const std::string ClockSlider::AttrName::Max("max");
const std::string ClockSlider::AttrName::Min("min");
const std::string ClockSlider::AttrName::Current("current");
const std::string ClockSlider::AttrName::DonutMachineName("machine");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ClockSlider, Element)
,max_(0)
,min_(100)
,current_(0)
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->edgeWidth(0);
	this->edgeColor(gl::Transparent);
	this->addAttribute(AttrName::Max, this->max_);
	this->addAttribute(AttrName::Min, this->min_);
	this->addAttribute(AttrName::Current, this->current_);
	this->addAttribute(AttrName::DonutMachineName, this->machineName_);
}

ClockSlider::~ClockSlider() noexcept
{
}

std::string ClockSlider::toString() const
{
	return ::cinamo::format("(ClockSlider min:\"%d\" max:\"%d\" now:\"%d\" %p)", this->min_, this->max_, this->current_, this);
}

void ClockSlider::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	Handler<World> w (this->world().lock());
	float const realWidth = innerSize().width()-10;
	int const logicalWidth = this->max_ - this->min_;
	geom::Distance offset(realWidth * ((float)this->current() / logicalWidth) - 2.5, 0);
	canvas.drawLine(3.0f, chisa::gl::Black, ptInRoot+geom::Distance(5.0, innerSize().height()/2), ptInRoot+geom::Distance(innerSize().width()-5.0, innerSize().height()/2));
	canvas.drawRect(5.0f, chisa::gl::DarkGray, geom::Area(ptInRoot+offset, geom::Box(5.0, innerSize().height())));
}

geom::Box ClockSlider::measureImpl(geom::Box const& constraint)
{
	return geom::Distance(20,20);
}

void ClockSlider::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
}

void ClockSlider::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	if( element->FirstChildElement() != nullptr ){
		this->message_ = XValue::fromXML(element->FirstChildElement());
	}
}

void ClockSlider::max(int const& max)
{
	this->max_ = max;
}

void ClockSlider::min(int const& min)
{
	this->min_ = min;
}

void ClockSlider::current(int const& current)
{
	this->current_ = current;
}

bool ClockSlider::notifyViewRefreshedImpl()
{
	return true;
}

bool ClockSlider::onSingleTapUp(const float& timeMs, const geom::Point& ptInScreen)
{
	if(!this->message_.is<XNull>()) {
		this->world().lock()->sendMessage(this->message_, this->machineName_);
	}
	return false;
}

}}
