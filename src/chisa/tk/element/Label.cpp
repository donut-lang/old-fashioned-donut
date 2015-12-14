/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <tinyxml2.h>
#include <cinamo/Exception.h>

#include "Label.hpp"
#include "../../gl/Canvas.hpp"
#include "../../gl/DrawableManager.hpp"
#include "../World.hpp"

#include "ElementGroup.hpp"
namespace chisa {
namespace tk {

const std::string Label::AttrName::Text("text");
const std::string Label::AttrName::FontSize("font-size");
const std::string Label::AttrName::DonutMachineName("machine");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Label, Element)
,text_()
,fontSize_(32.0f)
,vertical_(false)
,textImage_()
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->edgeWidth(0);
	this->edgeColor(gl::Transparent);
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::FontSize, this->fontSize_);
	this->addAttribute(AttrName::DonutMachineName, this->machineName_);
}

Label::~Label() noexcept
{
}

std::string Label::toString() const
{
	return ::cinamo::format("(Label text:\"%s\" %p)", this->text_.c_str(), this);
}

void Label::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	this->textImage()->draw(canvas, ptInRoot, mask);
}
Handler<gl::TextDrawable> Label::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock() ){
			if(vertical_){
				this->textImage_ = w->drawableManager()->queryVerticalText(
						this->text(),
						this->fontSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						this->foregroundColor(),
						gl::Transparent
						);
			}else{
				this->textImage_ = w->drawableManager()->queryText(
						this->text(),
						this->fontSize_,
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
geom::Box Label::measureImpl(geom::Box const& constraint)
{
	return this->textImage()->size();
}

void Label::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
}
void Label::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	if( element->FirstChildElement() != nullptr ){
		this->message_ = XValue::fromXML(element->FirstChildElement());
	}
}

void Label::text(std::string const& text)
{
	if(this->text_ != text){
		this->text_ = text;
		this->notifyViewRefreshed();
	}
}

void Label::textSize(float const& size)
{
	if( this->fontSize_ != size ) {
		this->fontSize_ = size;
		this->notifyViewRefreshed();
	}
}
void Label::setVertical(bool isVertical)
{
	if( this->vertical_ != isVertical ) {
		this->vertical_ = isVertical;
		this->notifyViewRefreshed();
	}
}

bool Label::notifyViewRefreshedImpl()
{
	this->textImage_.reset();
	return true;
}

bool Label::onSingleTapUp(const float& timeMs, const geom::Point& ptInScreen)
{
	if(!this->message_.is<XNull>()) {
		this->world().lock()->sendMessage(this->message_, this->machineName_);
	}
	return false;
}

}}
