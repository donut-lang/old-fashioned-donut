/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../String.h"

namespace cinamo {

template <typename T> XArchiver& XArchiver::operator &(T& val) {
	if(decode_now_){
		XSerializer<T>::deserialize(val, array_->get<XValue>(this->array_index_++));
	}else{
		array_->append(  XSerializer<T>::serialize(val) );
	}
	return *this;
}

template <typename T>
struct XSerializer<std::vector<T> > {
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

template <typename T, typename U>
struct XSerializer<std::pair<T,U> > {
	static XValue serialize(std::pair<T,U>& p) {
		Handler<XArray> array(new XArray);
		array->append( XSerializer<T>::serialize(p.first) );
		array->append( XSerializer<U>::serialize(p.second) );
		return array;
	}
	static void deserialize(std::pair<T,U>& p, XValue const& xval){
		Handler<XArray> array(xval.as<XArray>());
		if(array->size() != 2) {
			CINAMO_EXCEPTION(Exception, "Pair requested, but actual array size is %d", array->size());
		}
		XSerializer<T>::deserialize(p.first, array->get<XValue>(0));
		XSerializer<U>::deserialize(p.second, array->get<XValue>(1));
	}
};

template <>
struct XSerializer<XValue> {
	static XValue serialize(XValue& v) {
		return v;
	}
	static void deserialize(XValue& p, XValue const& xval){
		p = xval;
	}
};

template <typename T, std::size_t N>
struct XSerializer<T[N]> {
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
			CINAMO_EXCEPTION(Exception, "Array size does not match!: requested:%d != actual:%d", N, array->size());
		}
		int cnt=0;
		for( XValue& v : *(array) ) {
			XSerializer<T>::deserialize(val[cnt++], v);
		}
	}
};

template <std::size_t N>
struct XSerializer<char[N]> {
	static XValue serialize(char (&val)[N]) {
		return XValue( val, N);
	}
	static void deserialize(char (&val)[N], XValue const& xval){
		std::vector<char> c ( xval.as<XBinary>() );
		if(c.size() != N) {
			CINAMO_EXCEPTION(Exception, "Binary size does not match!: requested:%d != actual:%d", N, c.size());
		}
		std::copy(c.begin(), c.end(), val);
	}
};

template <std::size_t N>
struct XSerializer<unsigned char[N]> {
	static XValue serialize(unsigned char (&val)[N]) {
		return XValue( reinterpret_cast<char*>(val), N );
	}
	static void deserialize(unsigned char (&val)[N], XValue const& xval){
		std::vector<char> c ( xval.as<XBinary>() );
		if(c.size() != N) {
			CINAMO_EXCEPTION(Exception, "Binary size does not match!: requested:%d != actual:%d", N, c.size());
		}
		std::copy(c.begin(), c.end(), val);
	}
};

#define PRIM_VAR(TYPE)\
template <>\
struct XSerializer<TYPE> {\
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
PRIM_VAR(XBinary);
PRIM_VAR(Handler<XObject>);
PRIM_VAR(Handler<XArray>);

#undef PRIM_VAR

#define PRIMITIVE_PROXY(FROM, TO) \
template <>\
struct XSerializer<FROM, void> {\
	static XValue serialize(FROM& val) {\
		return XValue( static_cast<TO>(val) );\
	}\
	static void deserialize(FROM& val, XValue const& xval){\
		val = static_cast<FROM>(xval.as<TO>());\
	}\
};

PRIMITIVE_PROXY(unsigned char, XUInt);
PRIMITIVE_PROXY(char, XSInt);
PRIMITIVE_PROXY(signed char, XSInt);

PRIMITIVE_PROXY(unsigned short, XUInt);
PRIMITIVE_PROXY(short, XSInt);

PRIMITIVE_PROXY(unsigned long, XUInt);
PRIMITIVE_PROXY(long, XSInt);


}
