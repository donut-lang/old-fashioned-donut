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
		std::is_base_of<Object, typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type >::value &&
		!std::is_base_of<typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type, Object>::value
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
	std::is_base_of<typename std::remove_pointer<typename DecodeTraits<T>::return_type>::type, Object>::value
		>::type> final {
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
