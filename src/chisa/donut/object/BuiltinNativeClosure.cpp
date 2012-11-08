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
#include "Object.h"
#include "../Exception.h"

namespace chisa {
namespace donut {

const std::string TAG("BuiltinNativeClosure");

std::string BuiltinNativeClosure::toStringImpl() const
{
	return util::format("(BuiltinNativeClosure %p)", this);
}

int BuiltinNativeClosure::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to int.");
}

float BuiltinNativeClosure::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to float.");
}

bool BuiltinNativeClosure::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to bool.");
}

bool BuiltinNativeClosure::haveImpl(const std::string& name) const
{
	return false;
}

Handler<Object> BuiltinNativeClosure::storeImpl(const std::string& name, Handler<Object> obj)
{
	return obj;
}

Handler<Object> BuiltinNativeClosure::loadImpl(const std::string& name)
{
	throw DonutException(__FILE__, __LINE__, "builtin native closure does not have any property.");
}


}}
