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

#include "World.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("IntProxy");

IntProxy::IntProxy(World* const world)
:Proxy(world)
{
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

bool IntProxy::have(const Object* ptr, const std::string& name) const
{
}

Handler<Object> IntProxy::store(const Object* ptr, const std::string& name, Handler<Object> obj)
{
	this->world()->log().w(TAG, "Failed to store value to int object.");
	return obj;
}

Handler<Object> IntProxy::load(const Object* ptr, const std::string& name)
{
}

}}
