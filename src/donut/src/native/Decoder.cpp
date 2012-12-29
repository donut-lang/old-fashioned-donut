/**
 * Donut
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

#include <donut/native/Decoder.h>

namespace donut {
namespace native {

template <>
int decode<int>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toInt(heap);
}

template <>
unsigned int decode<unsigned int>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toInt(heap);
}
template <>
float decode<float>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toFloat(heap);
}

template <>
bool decode<bool>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toBool(heap);
}

template <>
std::string decode<std::string>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toString(heap);
}

}}
