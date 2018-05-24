/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <string>
#include <typeinfo>
#include <type_traits>
#include "Common.h"

namespace cinamo {

/**
 * typeinfoからクラス名のdemangleしたものを返す。
 */
std::string demangle(std::type_info const& info);

template <typename T>
std::string demangle(
		typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value >::type* = nullptr
)
{
	return demangle(typeid(T));
}

//ポインタが全部取れるまで再帰
template <typename T>
std::string demangle(typename std::enable_if<std::is_pointer<T>::value >::type* = nullptr)
{
	return demangle<typename std::remove_pointer<T>::type>();
}

//リファレンスが取れるまで再帰
template <typename T>
std::string demangle(typename std::enable_if<std::is_reference<T>::value >::type* = nullptr)
{
	return demangle<typename std::remove_reference<T>::type>();
}
template <typename T>
std::string demangle( T obj, typename std::enable_if<!std::is_pointer<T>::value >::type* = nullptr)
{
	return demangle(typeid(obj));
}

//ポインタを取り過ぎるとポリモルフィズムに対応できなくなっちゃうので再帰
template <typename T>
std::string demangle( T obj, typename std::enable_if<
		std::is_pointer<T>::value &&
		std::is_pointer<typename std::remove_pointer<T>::type >::value
			>::type* = nullptr)
{
	return demangle(*obj);
}

template <typename T>
std::string demangle( T obj, typename std::enable_if<
		std::is_pointer<T>::value &&
		!std::is_pointer<typename std::remove_pointer<T>::type >::value
			>::type* = nullptr)
{
	return demangle(typeid(*obj));
}

}
