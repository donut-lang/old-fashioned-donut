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

#define INT_DECODER(TYPE)\
template <>\
TYPE decode<TYPE>(Handler<Heap> const& heap, Handler<Object> obj)\
{\
	return obj->toInt(heap);\
}
INT_DECODER(long);
INT_DECODER(unsigned long);
INT_DECODER(int);
INT_DECODER(unsigned int);
INT_DECODER(short);
INT_DECODER(unsigned short);
INT_DECODER(char);
INT_DECODER(unsigned char);
INT_DECODER(signed char);
#define FLOAT_DECODER(TYPE)\
template <>\
TYPE decode<TYPE>(Handler<Heap> const& heap, Handler<Object> obj)\
{\
	return obj->toFloat(heap);\
}

FLOAT_DECODER(float);
FLOAT_DECODER(double);
FLOAT_DECODER(long double);

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
