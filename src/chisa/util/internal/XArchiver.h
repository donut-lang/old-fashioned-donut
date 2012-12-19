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
		XSerializer<T>::deserialize(val, array_->get<XValue>(this->array_index_++));
	}else{
		array_->append(  XSerializer<T>::serialize(val) );
	}
	return *this;
}

template <typename T>
struct XSerializer<std::vector<T>,0> {
	static XValue serialize(std::vector<T>& val){
		Handler<XArray> array(new XArray);
		for(T& v : val){
			array->append( XSerializer<T>::serialize(v) );
		}
		return array;
	}
	static void deserialize(std::vector<T>& val, XValue const& xval){
		Handler<XArray> array(xval.as<XArray>());
		val.clear();
		val.reserve( array->size() );
		for( XValue& v : *(array) ) {
			T t;
			XSerializer<T>::deserialize(t, v);
			val.push_back(  t );
		}
	}
};

template <typename T, std::size_t N>
struct XSerializer<T[N],0> {
	static XValue serialize(T (&val)[N]) {
		Handler<XArray> array(new XArray);
		for(T& v : val){
			array->append( XSerializer<T>::serialize(v) );
		}
		return array;
	}
	static void deserialize(T (&val)[N], XValue const& xval){
		Handler<XArray> array(xval.as<XArray>());
		if(array->size() != N) {
			std::size_t const n = N;
			throw logging::Exception(__FILE__, __LINE__, "Array size does not match!: requested:%d != actual:%d", n, array->size());
		}
		int cnt=0;
		for( XValue& v : *(array) ) {
			XSerializer<T>::deserialize(val[cnt++], v);
		}
	}
};

#define PRIM_VAR(TYPE)\
template <>\
struct XSerializer<TYPE,0> {\
	typedef typename _TypeAdapter<TYPE>::init_type Type;\
	static XValue serialize(Type& val){\
		return XValue(val);\
	}\
	static void deserialize(Type& val, XValue const& xval){\
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
