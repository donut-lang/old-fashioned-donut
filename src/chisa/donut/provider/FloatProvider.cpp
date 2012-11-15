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

namespace chisa {
namespace donut {

static const std::string TAG("FloatProvider");

FloatProvider::FloatProvider(World* world)
:NativeObjectProvider(world, "Float")
{
	this->registerPureNativeClosure("opAdd", std::function<float(float, float)>([&](float self, float v) {
		return self+v;
	}));
	this->registerPureNativeClosure("opSub", std::function<float(float, float)>([&](float self, float v) {
		return self-v;
	}));
	this->registerPureNativeClosure("opMul", std::function<float(float, float)>([&](float self, float v) {
		return self*v;
	}));
	this->registerPureNativeClosure("opDiv", std::function<float(float, float)>([&](float self, float v) {
		return self/v;
	}));
	this->registerPureNativeClosure("opPlus", std::function<float(float)>([&](float self) {
		return self;
	}));
	this->registerPureNativeClosure("opMinus", std::function<float(float)>([&](float self) {
		return -self;
	}));
	this->registerPureNativeClosure("opLt", std::function<float(float, float)>([&](float self, float v) {
		return self < v;
	}));
	this->registerPureNativeClosure("opLe", std::function<float(float, float)>([&](float self, float v) {
		return self <= v;
	}));
	this->registerPureNativeClosure("opGt", std::function<float(float, float)>([&](float self, float v) {
		return self > v;
	}));
	this->registerPureNativeClosure("opGe", std::function<float(float, float)>([&](float self, float v) {
		return self >= v;
	}));
	this->registerPureNativeClosure("opEq", std::function<float(float, float)>([&](float self, float v) {
		return self == v;
	}));
	this->registerPureNativeClosure("opNe", std::function<float(float, float)>([&](float self, float v) {
		return self != v;
	}));
	this->registerPureNativeClosure("opNe", std::function<int(float)>([&](float self) {
		return static_cast<int>(self);
	}));
}


tinyxml2::XMLElement* FloatProvider::serializeImpl( tinyxml2::XMLDocument* doc, Handler<Object> obj )
{
	tinyxml2::XMLElement* elm = doc->NewElement("float");
	elm->SetAttribute("val", obj->toFloat(world()));
	return elm;
}
Handler<Object> FloatProvider::deserializeImpl( tinyxml2::XMLElement* xml )
{
	if( std::string("int") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"bool\"", xml->Name());
	}
	float val;
	if(xml->QueryFloatAttribute("val", &val) != tinyxml2::XML_SUCCESS){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. failed to read xml");
	}
	return world()->createFloatObject(val);
}

}}
