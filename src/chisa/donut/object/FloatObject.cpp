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

const std::string TAG("FloatObject");
FloatObject::FloatObject(const Handler<Heap>& heap, const float& val)
: NativeObject(heap, "Float"), value_(val)
{
}

std::string FloatObject::toStringImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to String implicitly. Use toString()");
}

int FloatObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Integer implicitly. Use toInteger()");
}

float FloatObject::toFloatImpl(const Handler<Heap>& heap) const
{
	return this->value_;
}

bool FloatObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Float implicitly. Use toBoolean()");
}
void FloatObject::discardHistoryImpl(const Handler<Heap>& heap)
{

}

void FloatObject::discardFutureImpl(const Handler<Heap>& heap)
{

}

}}

