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

#include "Encoder.h"

#include "../object/Heap.h"
namespace chisa {
namespace donut {
namespace native {

template <>
Handler<Object> encode<int>(const Handler<Heap>& heap, int val)
{
	return heap->createInt(val);
}

template <>
Handler<Object> encode<float>(const Handler<Heap>& heap, float val)
{
	return heap->createFloatObject(val);
}

template <>
Handler<Object> encode<bool>(const Handler<Heap>& heap, bool val)
{
	return heap->createBool(val);
}

template <>
Handler<Object> encode<std::string>(const Handler<Heap>& heap, std::string val)
{
	return heap->createStringObject(val);
}

}}}


