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

#pragma once
#include "../object/Object.h"

namespace donut {
using namespace tarte;
class Heap;

namespace native {

template <typename T, typename U=void>
struct DecodeTraits {
	typedef T return_type;
};

template <typename T>
struct DecodeTraits<T, typename std::enable_if< std::is_reference<T>::value >::type> {
	typedef typename DecodeTraits<typename std::remove_reference<T>::type >::return_type return_type;
};

template <typename T>
struct DecodeTraits<T, typename std::enable_if< std::is_const<T>::value >::type> {
	typedef typename DecodeTraits<typename std::remove_const<T>::type >::return_type return_type;
};

template <typename T>
T decode(Handler<Heap> const& heap, Handler<Object> obj);

//-----------------------------------------------------------------------------

template <typename T, typename U=void>
struct Decoder final {
	static typename DecodeTraits<T>::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return decode<typename DecodeTraits<T>::return_type>(heap, obj);
	}
};

//ポインタだが、Objectの派生でない
template <typename T>
struct Decoder<T, typename std::enable_if<
	std::is_pointer<typename DecodeTraits<T>::return_type>::value &&
	!std::is_base_of<Object, typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type
		>::value >::type> final {
	static typename DecodeTraits<T>::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return decode<typename DecodeTraits<T>::return_type>(heap, obj);
	}
};

//ポインタでオブジェクトの派生
template <typename T>
struct Decoder<T, typename std::enable_if<
		std::is_pointer<typename DecodeTraits<T>::return_type>::value &&
		std::is_base_of<Object, typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type >::value
			>::type> final {
	static typename DecodeTraits<T>::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return obj.cast<typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type>().get();
	}
};

//ポインタでオブジェクトそのもの
template <typename T>
struct Decoder<T, typename std::enable_if<
	std::is_pointer<typename DecodeTraits<T>::return_type>::value &&
	std::is_base_of<Object, typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type>::value &&
	std::is_base_of<typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type, Object>::value >::type > final {
	static typename DecodeTraits<T>::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return obj.get();
	}
};
//ハンドラでオブジェクトそのもの
template <typename T>
struct Decoder< Handler<T>, typename std::enable_if<
		std::is_base_of<Object, typename DecodeTraits<T>::return_type >::value &&
		std::is_base_of<typename DecodeTraits<T>::return_type,Object>::value
			>::type > final {
	static typename DecodeTraits< Handler<T> >::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return obj;
	}
};

//ハンドラでオブジェクトの派生
template <typename T>
struct Decoder< Handler<T>, typename std::enable_if<
		std::is_base_of<Object, typename DecodeTraits<T>::return_type >::value &&
		!std::is_base_of<typename DecodeTraits<T>::return_type,Object>::value >::type
			> final {
	static typename DecodeTraits<Handler<T> >::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return obj.cast<T>();
	}
};

//ハンドラでオブジェクトと全く関係ない
template <typename T>
struct Decoder< Handler<T>, typename std::enable_if<
		!std::is_base_of<Object, typename DecodeTraits<T>::return_type >::value &&
		!std::is_base_of<typename DecodeTraits<T>::return_type,Object>::value
			>::type > final {
	static typename DecodeTraits<Handler<T> >::return_type exec(Handler<Heap> const& heap, Handler<Object> obj)
	{
		return decode<Handler<T> >( heap, obj );
	}
};

}}
