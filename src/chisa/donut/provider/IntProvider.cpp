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

static const std::string TAG("IntProvider");

IntProvider::IntProvider(World* world)
:NativeObjectProvider(world, "Integer")
{
	this->registerPureNativeClosure("opAdd", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return IntProvider::toPointer(IntProvider::fromPointer(self) + v);
	}));
	this->registerPureNativeClosure("opSub", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return IntProvider::toPointer(IntProvider::fromPointer(self) - v);
	}));
	this->registerPureNativeClosure("opMul", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return IntProvider::toPointer(IntProvider::fromPointer(self) * v);
	}));
	this->registerPureNativeClosure("opDiv", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return IntProvider::toPointer(IntProvider::fromPointer(self) / v);
	}));
	this->registerPureNativeClosure("opPlus", std::function<Object*(Object*)>([&](Object* self)->Object* {
		return IntProvider::toPointer(+IntProvider::fromPointer(self));
	}));
	this->registerPureNativeClosure("opMinus", std::function<Object*(Object*)>([&](Object* self)->Object* {
		return IntProvider::toPointer(-IntProvider::fromPointer(self));
	}));
	this->registerPureNativeClosure("opMod", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return IntProvider::toPointer(IntProvider::fromPointer(self) % v);
	}));
	this->registerPureNativeClosure("opLt", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) < v);
	}));
	this->registerPureNativeClosure("opLe", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) <= v);
	}));
	this->registerPureNativeClosure("opGt", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) > v);
	}));
	this->registerPureNativeClosure("opGe", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) >= v);
	}));
	this->registerPureNativeClosure("opEq", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) == v);
	}));
	this->registerPureNativeClosure("opNe", std::function<Object*(Object*, int)>([&](Object* self, int v)->Object* {
		return BoolProvider::toPointer(IntProvider::fromPointer(self) != v);
	}));
}

std::string IntProvider::toString(const Object* ptr) const
{
	return util::format("%d", fromPointer(ptr));
}

int IntProvider::toInt(const Object* ptr) const
{
	return fromPointer(ptr);
}

float IntProvider::toFloat(const Object* ptr) const
{
	return fromPointer(ptr);
}

bool IntProvider::toBool(const Object* ptr) const
{
	return fromPointer(ptr) != 0;
}

Handler<Object> IntProvider::create( const int& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( IntProvider::toPointer(val) );
}

tinyxml2::XMLElement* IntProvider::serializeImpl( tinyxml2::XMLDocument* doc, Handler<Object> obj )
{
	tinyxml2::XMLElement* elm = doc->NewElement("int");
	elm->SetAttribute("val", IntProvider::fromPointer(obj.get()));
	return elm;
}
Handler<Object> IntProvider::deserializeImpl( tinyxml2::XMLElement* xml )
{
	if( std::string("int") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"bool\"", xml->Name());
	}
	int val;
	if(xml->QueryIntAttribute("val", &val) != tinyxml2::XML_SUCCESS){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. failed to read xml");
	}
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( IntProvider::toPointer(val) );
}

}}
