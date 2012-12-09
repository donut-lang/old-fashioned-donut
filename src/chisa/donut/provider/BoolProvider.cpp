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
#include "../Exception.h"
#include <tinyxml2.h>
#include "../object/Heap.h"
#include "../native/Bind.h"

namespace chisa {
namespace donut {
static const std::string TAG("BoolProvider");

BoolProvider::BoolProvider(const Handler<Heap>& heap)
:Provider(heap, "Boolean")
{
	this->registerPureNativeClosure("opAnd", std::function<bool(bool, bool)>(
			[&](bool self, bool v)->bool{
		return self && v;
	}));
	this->registerPureNativeClosure("opOr", std::function<bool(bool, bool)>(
			[&](bool self, bool v)->bool{
		return self || v;
	}));
	this->registerPureNativeClosure("opNot", std::function<bool(bool)>(
			[&](bool self)->bool{
		return !self;
	}));
	this->registerPureNativeClosure("toString", std::function<std::string(bool)>(
			[&](bool self)->std::string{
		return util::toString(self);
	}));
}

std::string BoolProvider::toString(const Object* ptr) const
{
	return util::toString(fromPointer(ptr));
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
	return BoolProvider::fromPointer(ptr);
}

Handler<Object> BoolProvider::create( const bool& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( BoolProvider::toPointer(val) );
}

}}
