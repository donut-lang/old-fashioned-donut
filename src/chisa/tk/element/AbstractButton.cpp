/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <tinyxml2.h>
#include <cinamo/Exception.h>

#include "AbstractButton.hpp"
#include "../../gl/Canvas.hpp"
#include "../../gl/DrawableManager.hpp"
#include "../World.hpp"

#include "ElementGroup.hpp"

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
	this->margin(geom::Space(3.0f));
	this->padding(geom::Space(2.0f));
	this->edgeWidth(1);
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
}

AbstractButton::~AbstractButton() noexcept
{
}

std::string AbstractButton::toString() const
{
	return ::cinamo::format("(AbstractButton text:\"%s\" %p)", this->text_.c_str(), this);
}

void AbstractButton::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	if( this->enabled() ){
		if(this->isOn()) {
			this->renderOn(canvas, ptInRoot, mask);
		}else{
			this->renderOff(canvas, ptInRoot, mask);
		}
	} else {
		this->renderDisabled(canvas, ptInRoot, mask);
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
						enabled() ? this->foregroundColor() : disabledForegroundColor(),
						gl::Transparent
						);
			}else{
				this->textImage_ = w->drawableManager()->queryText(
						this->text(),
						this->textSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						enabled() ? this->foregroundColor() : disabledForegroundColor(),
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

void AbstractButton::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	this->layoutButtonContent(size);
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

bool AbstractButton::onDownRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	++this->pushedCnt_;
	return false;
}

bool AbstractButton::onUpRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	--this->pushedCnt_;
	return false;
}

bool AbstractButton::onMoveRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	return true;
}

void AbstractButton::enabledImpl(const bool& newstate)
{
	Element::enabledImpl(newstate);
	notifyViewRefreshed();
}

bool AbstractButton::onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen)
{
	if(this->lastInnerDrawnAreaInRoot().contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}
