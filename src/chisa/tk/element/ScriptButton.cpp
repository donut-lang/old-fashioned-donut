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
	return ::tarte::format("(ScriptButton text:\"%s\" %p)", this->text().c_str(), this);
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
			TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
		}
		if(this->log().d()){
			this->log().d(TAG, "Sending message on \"%s\"", this->machineName_.c_str());
		}
		world->sendMessage(this->message_);
	}else{
		if(this->log().d()){
			this->log().d(TAG, "Button script is empty.");
		}
	}
}

}}
