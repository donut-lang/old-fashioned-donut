/**
 * Tarte
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

#include <string>
#include <typeinfo>
#include "Common.h"

namespace tarte {

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
