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

namespace chisa {
namespace util {

template <typename T> XArchiver& XArchiver::operator &(T& val) {
	if(decode_now_){
		XDeserializer<T>::exec(val, array_->get<XValue>(count_++));
	}else{
		array_->append(  XSerializer<T>::exec(val) );
	}
	return *this;
}

#define PRIM_VAR(TYPE)\
template <>\
struct XSerializer<TYPE, false> {\
	typedef typename _TypeAdapter<TYPE>::init_type Type;\
	static XValue exec(Type& val){\
		return XValue(val);\
	}\
};\
template <>\
struct XDeserializer<TYPE, false> {\
	typedef typename _TypeAdapter<TYPE>::init_type Type;\
	static void exec(Type& val, XValue const& xval){\
		val=xval.as<Type>();\
	}\
};

PRIM_VAR(XNull);
PRIM_VAR(XString);
PRIM_VAR(XUInt);
PRIM_VAR(XSInt);
PRIM_VAR(XFloat);
PRIM_VAR(XBool);
PRIM_VAR(XArray);
PRIM_VAR(XObject);

#undef PRIM_VAR

}}
