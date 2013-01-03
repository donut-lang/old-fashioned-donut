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

#include <algorithm>
#include "../EnumProxy.h"
#include "../Exception.h"

namespace tarte {

class XArray;
class XObject;

template <> struct _TypeAdapter<XArray> {
	typedef XArray spirit_type;
	typedef Handler<XArray> init_type;
	typedef Handler<XArray>& return_type;
	typedef Handler<XArray> const& return_const_type;
};

template <> struct _TypeAdapter<XObject> {
	typedef XObject spirit_type;
	typedef Handler<XObject> init_type;
	typedef Handler<XObject>& return_type;
	typedef Handler<XObject> const& return_const_type;
};

#define DEF_TYPE(TYPE)\
template <> struct _TypeAdapter<TYPE> {\
	typedef TYPE spirit_type;\
	typedef TYPE init_type;\
	typedef TYPE& return_type;\
	typedef TYPE const& return_const_type;\
};

DEF_TYPE(XValue);
DEF_TYPE(XNull);
DEF_TYPE(XSInt);
DEF_TYPE(XUInt);
DEF_TYPE(XFloat);
DEF_TYPE(XBool);
DEF_TYPE(XString);
DEF_TYPE(XBinary);

#undef DEF_TYPE

#define PROXY_ADAPTER(P_TYPE)\
	typedef typename _TypeAdapter<P_TYPE>::spirit_type spirit_type;\
	typedef typename _TypeAdapter<P_TYPE>::init_type init_type;\
	typedef typename _TypeAdapter<P_TYPE>::return_type return_type;\
	typedef typename _TypeAdapter<P_TYPE>::return_const_type return_const_type;\

template <typename T> struct _TypeAdapter<T&> { PROXY_ADAPTER(T) };
template <typename T> struct _TypeAdapter<T const&> { PROXY_ADAPTER(T) };
template <typename T> struct _TypeAdapter<T const> { PROXY_ADAPTER(T) };
template <typename T> struct _TypeAdapter<T, typename std::enable_if<std::is_enum<T>::value>::type> {
	typedef typename _TypeAdapter<XSInt>::spirit_type spirit_type;
	typedef typename _TypeAdapter<XSInt>::init_type init_type;
	typedef EnumProxy<T>      return_type;
	typedef ConstEnumProxy<T> return_const_type;
};
template <> struct _TypeAdapter<Handler<XArray> > { PROXY_ADAPTER(XArray) };
template <> struct _TypeAdapter<Handler<XObject> > { PROXY_ADAPTER(XObject) };
template <> struct _TypeAdapter<const char*> { PROXY_ADAPTER(XString) };
template <std::size_t N> struct _TypeAdapter<const char[N]> { PROXY_ADAPTER(XString) };
template <std::size_t N> struct _TypeAdapter<char[N]> { PROXY_ADAPTER(XString) };
template <> struct _TypeAdapter<unsigned char> { PROXY_ADAPTER(XUInt) };
template <> struct _TypeAdapter<char> { PROXY_ADAPTER(XSInt) };
template <> struct _TypeAdapter<unsigned short> { PROXY_ADAPTER(XUInt) };
template <> struct _TypeAdapter<short> { PROXY_ADAPTER(XSInt) };

#undef PROXY_ADAPTER

template <typename T> typename _TypeAdapter<T>::return_type XArray::get( std::size_t const& idx ) {
	if( idx >= list_.size() ){
		TARTE_EXCEPTION(Exception, "idx: %d >= size: %d ", idx, list_.size());
	}
	XValue& val = list_[idx];
	if( !val.is<T>() ) {
		TARTE_EXCEPTION(Exception, "Array: \"%d\" type mismatch required: %s actual: %s.", idx, XValue(typename _TypeAdapter<T>::init_type()).typeString().c_str(), val.typeString().c_str());
	}
	return val.as<T>();
}

template <typename T> typename _TypeAdapter<T>::return_type XArray::get( std::size_t const& idx, T& v ) {
	typename _TypeAdapter<T>::return_type const r = this->get<T>(idx);
	v = r;
	return r;
}

template <typename T> bool XArray::has( std::size_t const& idx ) const {
	if( idx >= list_.size() ){
		return false;
	}
	XValue const& val = list_[idx];
	return val.is<T>();
}
template <typename T> typename _TypeAdapter<T>::return_type XArray::set( std::size_t const& idx, T const& obj ) {
	if( idx >= list_.size() ){
		TARTE_EXCEPTION(Exception, "idx: %d >= size: %d ", idx, list_.size());
	}
	XValue& val = list_[idx];
	val = XValue(obj);
	return val.as<T>();
}
template <typename T> typename _TypeAdapter<T>::return_type XArray::append( T const& obj ) {
	this->list_.push_back(XValue(obj));
	return this->list_.back().as<T>();
}

template<typename T> typename _TypeAdapter<T>::return_type XObject::get( std::string const& name ) {
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name){
		TARTE_EXCEPTION(Exception, "Object: \"%s\" not found.", name.c_str());
	}
	if( !p.second.is<T>() ) {
		TARTE_EXCEPTION(Exception, "Object: \"%s\" type mismatch required: %s actual: %s.", name.c_str(), XValue(typename _TypeAdapter<T>::init_type()).typeString().c_str(), it->second.typeString().c_str());
	}
	return p.second.as<T>();
}

template<typename T> typename _TypeAdapter<T>::return_type XObject::get( std::string const& name, T& v ) {
	typename _TypeAdapter<T>::return_type const r = this->get<T>(name);
	v = r;
	return r;
}

template<> inline XValue& XObject::get<XValue>( std::string const& name ) {
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name){
		TARTE_EXCEPTION(Exception, "Object: \"%s\" not found.", name.c_str());
	}
	return p.second;
}

template<typename T> typename _TypeAdapter<T>::return_type XObject::opt(std::string const& name)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue> const& p = *it;
	if( it == this->map_.end() || p.first != name || !p.second.is<T>()){
		return T();
	}
	return p.second.as<T>();
}

template<typename T> bool XObject::has(std::string const& name)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue> const& p = *it;
	if( it == this->map_.end() || p.first != name || !p.second.is<T>()){
		return false;
	}
	return true;
}
template<typename T> typename _TypeAdapter<T>::return_type XObject::set(std::string const& name, T const& obj)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name) {
		it = this->map_.insert(it, std::pair<std::string, XValue>(name, XValue(obj)));
		return it->second.as<T>();
	}else{
		p.second = XValue(obj);
		return p.second.as<T>();
	}
}


#define FUNCT(TYPE, VAL) \
		template <> inline bool XValue::isImpl<XValue::TYPE>() const noexcept {\
			return this->type_ == XValue::Type::TYPE##T;\
		};\
		template <> inline typename _TypeAdapter<XValue::TYPE>::return_type XValue::asImpl<XValue::TYPE>() {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				typedef typename _TypeAdapter<XValue::TYPE>::init_type Type;\
				TARTE_EXCEPTION(Exception, "Type mismatched! required: %s actual: %s.", XValue((Type())).typeString().c_str(), this->typeString().c_str());\
			}\
			return VAL;\
		};\
		template <> inline typename _TypeAdapter<XValue::TYPE>::return_const_type XValue::asImpl<XValue::TYPE>() const {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				typedef typename _TypeAdapter<XValue::TYPE>::init_type Type;\
				TARTE_EXCEPTION(Exception, "Type mismatched! required: %s actual: %s.", XValue((Type())).typeString().c_str(), this->typeString().c_str());\
			}\
			return VAL;\
		};\

FUNCT(Null, spirit_.null_);
FUNCT(Array, *spirit_.array_);
FUNCT(Object, *spirit_.object_);
FUNCT(String, *spirit_.str_);
FUNCT(Binary, *spirit_.binary_);
FUNCT(UInt, spirit_.uint_);
FUNCT(SInt, spirit_.int_);
FUNCT(Float, spirit_.float_);
FUNCT(Bool, spirit_.bool_);
#undef FUNCT

template <> inline bool XValue::isImpl<XValue>() const noexcept {
	return true;
};
template <> inline typename _TypeAdapter<XValue>::return_type XValue::asImpl<XValue>() {
	return *this;
};
template <> inline typename _TypeAdapter<XValue>::return_const_type XValue::asImpl<XValue>() const {
	return *this;
};

template <> inline XValue XValue::fromString<XValue::String>(std::string const& str)
{
	return XValue((String)str);
}

template <typename T>
inline bool XValue::is() const noexcept {
	return this->isImpl<typename _TypeAdapter<T>::spirit_type>();
};
template <typename T>
inline typename _TypeAdapter<T>::return_type XValue::as() {
	return this->asImpl<typename _TypeAdapter<T>::spirit_type>();
};
template <typename T>
inline typename _TypeAdapter<T>::return_const_type XValue::as() const {
	return this->asImpl<typename _TypeAdapter<T>::spirit_type>();
};

template <typename T>
inline typename _TypeAdapter<T>::return_type XValue::as(T& v) {
	typename _TypeAdapter<T>::return_type const r = this->as<T>();
	v = r;
	return r;
};
template <typename T>
inline typename _TypeAdapter<T>::return_const_type XValue::as(T& v) const {
	typename _TypeAdapter<T>::return_const_type const r = this->as<T>();
	v = r;
	return r;
};

template <> XValue& XObject::set<XValue>(std::string const& name, XValue const& obj);
template <> XValue XValue::fromString<XValue::UInt>(std::string const& str);
template <> XValue XValue::fromString<XValue::SInt>(std::string const& str);
template <> XValue XValue::fromString<XValue::Float>(std::string const& str);
template <> XValue XValue::fromString<XValue::Bool>(std::string const& str);
template <> XValue XValue::fromString<XValue::Binary>(std::string const& str);

}
