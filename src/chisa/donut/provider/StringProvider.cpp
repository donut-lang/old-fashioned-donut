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

#include "Provider.h"
#include "../../util/StringUtil.h"
#include "../object/World.h"
#include "../Exception.h"
#include <tinyxml2.h>
#include <sstream>

namespace chisa {
namespace donut {

static const std::string TAG("StringProvider");

StringProvider::StringProvider(World* world)
:NativeObjectProvider(world, "String")
{
	this->registerPureNativeClosure("toInteger", std::function<int(StringObject*)>([&](StringObject* self) {
		char* c;
		std::string const str = self->toString(this->world());
		int v = std::strtol(str.c_str(), &c, 0);
		if (*c) {
			throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to int.", str.c_str());
		}
		return v;
	}));
	this->registerPureNativeClosure("toFloat", std::function<float(StringObject*)>([&](StringObject* self) {
		char* c;
		std::string const str = self->toString(this->world());
		float v = std::strtof(str.c_str(), &c);
		if (*c) {
			throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to float.", str.c_str());
		}
		return v;
	}));
	this->registerPureNativeClosure("toBoolean", std::function<bool(StringObject*)>([&](StringObject* self) {
		std::string const str = self->toString(this->world());
		std::string copy(str);
		std::transform(copy.begin(), copy.end(), copy.begin(), (int (*)(int))std::tolower);
		if( copy == "true" || copy == "yes") {
			return true;
		}
		else if(copy == "false" || copy=="no") {
			return false;
		}
		throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to bool.", str.c_str());
	}));
	this->registerPureNativeClosure("opAdd", std::function<std::string(StringObject*,StringObject*)>([&](StringObject* self, StringObject* other) {
		std::string const str = self->toString(this->world());
		std::string const ostr = other->toString(this->world());
		return str + ostr;
	}));
	this->registerPureNativeClosure("opMul", std::function<std::string(StringObject*, int)>([&](StringObject* self, int times) {
		std::string const str = self->toString(this->world());
		std::stringstream ss;
		for(int i=0;i<times;++i){
			ss << str;
		}
		return ss.str();
	}));
}

tinyxml2::XMLElement* StringProvider::serializeImpl( tinyxml2::XMLDocument* doc, Handler<Object> obj )
{
	tinyxml2::XMLElement* elm = doc->NewElement("string");
	elm->SetAttribute("val", obj->toString(world()).c_str());
	return elm;
}
Handler<Object> StringProvider::deserializeImpl( tinyxml2::XMLElement* xml )
{
	if( std::string("string") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"string\"", xml->Name());
	}
	char const* val = nullptr;
	if(!(val = xml->Attribute("val"))){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. failed to read xml");
	}
	return world()->createStringObject(val);
}

}}
