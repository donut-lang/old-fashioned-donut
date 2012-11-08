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

#include "ObjectPool.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("NullProxy");

NullProxy::NullProxy(ObjectPool* const pool)
:pool_(pool)
{

}

std::string NullProxy::toString(const Object* ptr) const
{
	return "(null)";
}

int NullProxy::toInt(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert null to int.");
}

float NullProxy::toFloat(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert null to float.");
}

bool NullProxy::toBool(const Object* ptr) const
{
	return false;
}

bool NullProxy::have(const Object* ptr, const std::string& name) const
{
	return false;
}

Handler<Object> NullProxy::store(const Object* ptr, const std::string& name, Handler<Object> obj)
{
	throw DonutException(__FILE__, __LINE__, "Failed to store to null.");
}

Handler<Object> NullProxy::load(const Object* ptr, const std::string& name)
{
	throw DonutException(__FILE__, __LINE__, "Failed to load from null.");
}

}}
