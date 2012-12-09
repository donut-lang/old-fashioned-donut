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
#include "../Exception.h"
#include <tinyxml2.h>

namespace chisa {
namespace donut {

static const std::string TAG("NullProvider");

NullProvider::NullProvider(const Handler<Heap>& heap)
:Provider(heap, "Null")
{
}

std::string NullProvider::toString(const Object* ptr) const
{
	return "(null)";
}

int NullProvider::toInt(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert null to int.");
}

float NullProvider::toFloat(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert null to float.");
}

bool NullProvider::toBool(const Object* ptr) const
{
	return false;
}

Handler<Object> NullProvider::create()
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( NullProvider::toPointer() );
}

}}
