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

#include "ProxyImpl.h"
#include "../../Exception.h"

namespace chisa {
namespace donut {
static const std::string TAG("BoolProxy");

BoolProxy::BoolProxy(World* const world)
:Proxy(world)
{
	this->registerClosure("opAnd", std::function<Object*(Object*, bool)>(
			[&](Object* self, bool v)->Object*{
		return BoolProxy::toPointer(BoolProxy::fromPointer(self) && v);
	}));
	this->registerClosure("opOr", std::function<Object*(Object*, bool)>(
			[&](Object* self, bool v)->Object*{
		return BoolProxy::toPointer(BoolProxy::fromPointer(self) || v);
	}));
	this->registerClosure("opNot", std::function<Object*(Object*)>(
			[&](Object* self)->Object*{
		return BoolProxy::toPointer(!BoolProxy::fromPointer(self));
	}));
}

std::string BoolProxy::toString(const Object* ptr) const
{
	return fromPointer(ptr) ? "true" : "false";
}

int BoolProxy::toInt(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Ccannot convert from boolean to integer.");
}

float BoolProxy::toFloat(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Ccannot convert from boolean to float.");
}

bool BoolProxy::toBool(const Object* ptr) const
{
	return fromPointer(ptr);
}

Handler<Object> BoolProxy::store(const Object* ptr, const std::string& name, Handler<Object> obj)
{
	this->world()->log().w(TAG, "Failed to store value to bool object.");
	return obj;
}

}}
