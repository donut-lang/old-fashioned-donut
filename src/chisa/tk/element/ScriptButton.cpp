/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <tinyxml2.h>
#include <cinamo/Exception.h>

#include "ScriptButton.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

namespace chisa {
namespace tk {

static std::string const TAG("ScriptButton");
const std::string ScriptButton::AttrName::DonutMachineName("machine");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ScriptButton, ClickButton)
,machineName_()
,message_(nullptr)
{
	this->addAttribute(AttrName::DonutMachineName, this->machineName_);
}

ScriptButton::~ScriptButton() noexcept
{
}

std::string ScriptButton::toString() const
{
	return ::cinamo::format("(ScriptButton text:\"%s\" %p)", this->text().c_str(), this);
}

void ScriptButton::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	if( element->FirstChildElement() != nullptr ){
		this->message_ = XValue::fromXML(element->FirstChildElement());
	}

}

void ScriptButton::onClick()
{
	if(!this->message_.is<XNull>()){
		if(this->log().t()){
			this->log().t(TAG, "Sending message.");
		}
		Handler<World> world( this->world().lock() );
		if( unlikely(!world) ) {
			CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
		}
		if(this->log().d()){
			this->log().d(TAG, "Sending message on \"%s\"", this->machineName_.c_str());
		}
		world->sendMessage(this->message_, this->machineName_);
	}else{
		if(this->log().d()){
			this->log().d(TAG, "Button script is empty.");
		}
	}
}

}}
