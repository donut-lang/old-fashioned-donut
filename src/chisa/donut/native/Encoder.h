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

#pragma once
#include "../object/Object.h"


namespace chisa {
namespace donut {
class World;

namespace native {

template <typename T>
Handler<Object> encode(World* world, T obj);

//-----------------------------------------------------------------------------

template <typename T, bool = IsBaseOf<T, Object>::result >
struct PointerEncoder {
	static Handler<Object> exec(World* world, T* obj) {
		return encode<T*>( world, obj );
	}
};

template <typename T>
struct PointerEncoder<T, true> {
	static Handler<Object> exec(World* world, T* obj)
	{
		return Handler<Object>::__internal__fromRawPointerWithoutCheck( obj );
	}
};

//-----------------------------------------------------------------------------

template <typename T, bool = IsBaseOf<T, Object>::result >
struct HandlerEncoder {
	static Handler<Object> exec(World* world, Handler<T> obj)
	{
		return encode<Handler<T> >( world, obj );
	}
};

template <typename T>
struct HandlerEncoder<T, true> {
	static Handler<Object> exec(World* world, Handler<T> obj)
	{
		return obj;
	}
};

//-----------------------------------------------------------------------------

template <typename T>
struct Encoder{
	static Handler<Object> exec(World* world, T obj) {
		return encode<T>(world, obj);
	}
};

template <typename T>
struct Encoder<T*> {
	static Handler<Object> exec(World* world, T* obj)
	{
		return PointerEncoder<T>::exec(world, obj);
	}
};

template <typename T>
struct Encoder< Handler<T> > {
	static Handler<Object> exec(World* world, Handler<T> obj)
	{
		return HandlerEncoder<T>::exec( world, obj );
	}
};
}}}
