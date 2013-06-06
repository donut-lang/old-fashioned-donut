/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <tinyxml2.h>
#include <cinamo/Exception.h>

#include "ClockButton.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

namespace cinamo {
namespace xml {
template <>
void parseAttr<chisa::tk::ClockButton::ClockOperation>(std::string const& elmname, chisa::tk::ClockButton::ClockOperation& val, chisa::tk::ClockButton::ClockOperation const& def, tinyxml2::XMLElement* xml)
{
	const char* _str = xml->Attribute(elmname.c_str());
	if(!_str) {
		val = def;
		return;
	}
	std::string str(cinamo::toLower(_str));
	if(str == "forward") {
		val = chisa::tk::ClockButton::ClockOperation::Forward;
	}else  if(str=="back"){
		val = chisa::tk::ClockButton::ClockOperation::Back;
	}else{
		val = def;
	}
}

}}

namespace chisa {
namespace tk {

static std::string const TAG("ClockButton");

const std::string ClockButton::AttrName::Operation("op");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ClockButton, ClickButton)
,operation_(Back)
{
	this->addAttribute(AttrName::Operation, this->operation_);
}

ClockButton::~ClockButton() noexcept
{
}

std::string ClockButton::toString() const
{
	return ::cinamo::format("(ClockButton text:\"%s\" %p)", this->text().c_str(), this);
}

void ClockButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void ClockButton::onClick()
{
	switch(operation_){
	case Back: {
		Handler<World> w = this->world().lock();
		w->sendTask([w](){
			w->back();
		});
	}
		break;
	case Forward: {
		Handler<World> w = this->world().lock();
		w->sendTask([w](){
			w->forward();
		});
	}
		break;
	}
}

bool ClockButton::enabledImpl()
{
	Handler<World> w (this->world().lock());
	if( unlikely(!w) ){
		return false;
	}
	// FIXME: 見苦しい　elementが変化を見守るべきだと思う
	bool const clk = ( operation_ == Forward ?
			w->donut()->clock()->canAdvance() :
			w->donut()->clock()->canBack());
	bool const orig = this->Element::enabledImpl();
	if( clk ^ orig ){
		enabled(clk);
	}
	return clk;

}

}}
