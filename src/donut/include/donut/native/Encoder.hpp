/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../object/Object.hpp"

namespace donut {
using namespace cinamo;
class Heap;

namespace native {

template <typename T>
Handler<Object> encode(Handler<Heap> const& heap, T obj);

//-----------------------------------------------------------------------------

template <typename T, typename U=void>
struct Encoder final{
	static Handler<Object> exec(Handler<Heap> const& heap, T obj) {
		return encode<T>(heap, obj);
	}
};

//ポインタだが、Objectの派生でない
template <typename T>
struct Encoder<T, typename std::enable_if<
	std::is_pointer<T>::value &&
	!std::is_base_of<Object, typename std::remove_pointer<T>::type
		>::value >::type> final {
	static Handler<Object> exec(Handler<Heap> const& heap, T obj) {
		return encode<T>(heap, obj);
	}
};

//ポインタで、Objectの派生
template <typename T>
struct Encoder<T, typename std::enable_if<
	std::is_pointer<T>::value &&
	std::is_base_of<Object, typename std::remove_pointer<T>::type
		>::value >::type> final {
	static Handler<Object> exec(Handler<Heap> const& heap, T obj) {
		return Handler<Object>::__internal__fromRawPointerWithoutCheck( obj );
	}
};

//ハンドラだがオブジェクトではない
template <typename T>
struct Encoder< Handler<T>, typename std::enable_if<
		!std::is_base_of<Object, T>::value
			>::type> final {
	static Handler<Object> exec(Handler<Heap> const& heap, Handler<T> obj)
	{
		return encode<Handler<T> >( heap, obj );
	}
};

//ハンドラでオブジェクト
template <typename T>
struct Encoder< Handler<T>, typename std::enable_if<
		std::is_base_of<Object, T>::value
			>::type> final {
	static Handler<Object> exec(Handler<Heap> const& heap, Handler<T> obj)
	{
		return obj;
	}
};

}}
