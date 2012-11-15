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

#include "PrimitiveProvider.h"
#include "../Exception.h"
#include <tinyxml2.h>
#include "../object/World.h"
#include "../native/Bind.h"

namespace chisa {
namespace donut {
static const std::string TAG("BoolProvider");

BoolProvider::BoolProvider(World* world)
:NativeObjectProvider(world, "Boolean")
{
	/*
	this->registerPureNativeClosure("opAnd", native::createBind( std::function<Object*(Object*, bool)>(
			[&](Object* self, bool v)->Object*{
		return BoolProvider::toPointer(BoolProvider::fromPointer(self) && v);
	})));
	this->registerPureNativeClosure("opOr", native::createBind( std::function<Object*(Object*, bool)>(
			[&](Object* self, bool v)->Object*{
		return BoolProvider::toPointer(BoolProvider::fromPointer(self) || v);
	})));
	this->registerPureNativeClosure("opNot", native::createBind( std::function<Object*(Object*)>(
			[&](Object* self)->Object*{
		return BoolProvider::toPointer(!BoolProvider::fromPointer(self));
	})));
	*/
}

std::string BoolProvider::toString(const Object* ptr) const
{
	return fromPointer(ptr) ? "true" : "false";
}

int BoolProvider::toInt(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Ccannot convert from boolean to integer.");
}

float BoolProvider::toFloat(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Ccannot convert from boolean to float.");
}

bool BoolProvider::toBool(const Object* ptr) const
{
	return fromPointer(ptr);
}

tinyxml2::XMLElement* BoolProvider::serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj )
{
	tinyxml2::XMLElement* elm = doc->NewElement("bool");
	elm->SetAttribute("val", BoolProvider::fromPointer(obj.get()));
	return elm;
}
Handler<Object> BoolProvider::deserialize( tinyxml2::XMLElement* xml )
{
	if( std::string("bool") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"bool\"", xml->Name());
	}
	bool val;
	if(xml->QueryBoolAttribute("val", &val) != tinyxml2::XML_SUCCESS){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. failed to read xml");
	}
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( BoolProvider::toPointer(val) );
}

Handler<Object> BoolProvider::create( const bool& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( BoolProvider::toPointer(val) );
}

}}
