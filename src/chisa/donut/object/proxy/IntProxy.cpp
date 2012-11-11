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
#include "../../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("IntProxy");

IntProxy::IntProxy(World* const world)
:Proxy(world)
{
	this->registerClosure("opAdd", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) + v);
	}));
	this->registerClosure("opSub", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) - v);
	}));
	this->registerClosure("opMul", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) * v);
	}));
	this->registerClosure("opMod", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) % v);
	}));
	this->registerClosure("opLt", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) < v);
	}));
	this->registerClosure("opLe", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) <= v);
	}));
	this->registerClosure("opGt", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) > v);
	}));
	this->registerClosure("opGe", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) >= v);
	}));
	this->registerClosure("opEq", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) == v);
	}));
	this->registerClosure("opNe", std::function<Object*(Object*, const int&)>(
			[&](Object* self, const int& v)->Object*{
		return IntProxy::toPointer(IntProxy::fromPointer(self) != v);
	}));
}

std::string IntProxy::toString(const Object* ptr) const
{
	return util::format("%d", fromPointer(ptr));
}

int IntProxy::toInt(const Object* ptr) const
{
	return fromPointer(ptr);
}

float IntProxy::toFloat(const Object* ptr) const
{
	return fromPointer(ptr);
}

bool IntProxy::toBool(const Object* ptr) const
{
	return fromPointer(ptr) != 0;
}

Handler<Object> IntProxy::store(const Object* ptr, const std::string& name, Handler<Object> obj)
{
	this->world()->log().w(TAG, "Failed to store value to int object.");
	return obj;
}

}}
