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

#include "XVal.h"

namespace chisa {
namespace util {

template <typename T, bool has_serializer> struct XSerializer;
template <typename T, bool has_serializer> struct XDeserializer;

class XArchiver {
	DISABLE_COPY_AND_ASSIGN(XArchiver);
private:
	Handler<XArray> array_;
	bool decode_now_;
	size_t count_;
private:
	explicit XArchiver(Handler<XArray> const& a):array_(a), decode_now_(true), count_(0){}
	template <typename T, bool n> friend struct XDeserializer;
	template <typename T, bool n> friend struct XSerializer;
public:
	explicit XArchiver();
	explicit XArchiver(XValue const& val);
public:
	XValue data() {
		return array_;
	}
public:
	template <typename T> XArchiver& operator &(T& val);
	template <typename T> XArchiver& operator>>(T& t){
		return this->operator &(t);
	}
	template <typename T> XArchiver& operator<<(T& t){
		return this->operator &(t);
	}
};

template <typename T>
class HasSerializer {
	template <typename U>
	static auto check(U u) -> decltype(u.serialize(std::declval<XArchiver&>()), std::true_type());
	static auto check(...) -> decltype( std::false_type() );
public:
	typedef decltype(check(std::declval<T>())) check_func_decl;
	static const bool value = check_func_decl::value;
};
template <typename T, bool has_serializer=HasSerializer<T>::value >
struct XSerializer {
	static XValue exec(T& val){
		XArchiver a;
		val.serialize(a);
		return a.array_;
	}
};
template <typename T, bool has_serializer=HasSerializer<T>::value >
struct XDeserializer {
	static void exec(T& val, XValue const& xval){
		XArchiver arc(xval.as<XArray>());
		val.serialize(arc);
	}
};

template <typename T>
struct XSerializer<T,false > {
	static XValue exec(T& val);
};
template <typename T>
struct XDeserializer<T, false> {
	static void exec(T& val, XValue const& xval);
};

}}

#include "internal/XArchiver.h"
