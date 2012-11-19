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
#include <algorithm>

namespace chisa {
namespace donut {

const std::string TAG("StringObject");
StringObject::StringObject(Heap* heap, const std::string& str)
		: NativeObject(heap, "String"), str_(str)
{
}

std::string StringObject::toStringImpl() const
{
	return this->str_;
}

int StringObject::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast String to Integer implicitly. Use toInteger()");
}

float StringObject::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast String to Float implicitly. Use toFloat()");
}

bool StringObject::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast String to Float implicitly. Use toBoolean()");
}

}}

