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

#include "NativeObject.h"
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "../object/World.h"

namespace chisa {
namespace donut {
static const std::string TAG("NativeClosureObject");

std::string NativeClosureObject::toStringImpl() const
{
	return util::format("(NativeClosureObject %p)", this);
}

int NativeClosureObject::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to int.");
}

float NativeClosureObject::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to float.");
}

bool NativeClosureObject::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert builtin native closure to bool.");
}

bool NativeClosureObject::haveImpl(const std::string& name) const
{
	return false;
}

bool NativeClosureObject::haveOwnImpl(const std::string& name) const
{
	return false;
}

Handler<Object> NativeClosureObject::storeImpl(const std::string& name, Handler<Object> obj)
{
	return obj;
}

Handler<Object> NativeClosureObject::loadImpl(const std::string& name) const
{
	throw DonutException(__FILE__, __LINE__, "Native Closure does not have any properety.");
}


}}


