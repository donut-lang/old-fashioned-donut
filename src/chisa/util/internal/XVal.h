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

#include <algorithm>

namespace chisa {
namespace util {

class XArray;
class XObject;

template <typename T> struct _GetType {
	typedef T& type;
	typedef T const& const_type;
};
template <> struct _GetType<XArray> {
	typedef Handler<XArray>& type;
	typedef Handler<XArray> const& const_type;
};
template <> struct _GetType<XObject> {
	typedef Handler<XObject>& type;
	typedef Handler<XObject> const& const_type;
};

template <typename T> T& XArray::get( std::size_t const& idx ) {
	if( idx < 0 || idx >= list_.size() ){
		throw logging::Exception(__FILE__, __LINE__, "idx: %d >= size: %d ", idx, list_.size());
	}
	XValue& val = list_[idx];
	if( !val.is<T>() ) {
		throw logging::Exception(__FILE__, __LINE__, "Array: \"%d\" type mismatch required: %s actual: %s.", idx, typeid(T).name(), val.toString().c_str());
	}
	return val.as<T>();
}
template <typename T> T const& XArray::set( const std::size_t& idx, T const& obj ) {
	if( idx < 0 || idx >= list_.size() ){
		throw logging::Exception(__FILE__, __LINE__, "idx: %d >= size: %d ", idx, list_.size());
	}
	return list_[idx] = XValue(obj);
}
template <typename T> T const& XArray::append( T const& obj ) {
	this->list_.push_back(XValue(obj));
	return obj;
}

template<typename T> typename _GetType<T>::type XObject::get( std::string const& name ) {
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name){
		throw logging::Exception(__FILE__, __LINE__, "Object: \"%s\" not found.", name.c_str());
	}
	if( !p.second.is<T>() ) {
		throw logging::Exception(__FILE__, __LINE__, "Object: \"%s\" type mismatch required: %s actual: %s.", typeid(T).name(), it->second.toString().c_str());
	}
	return p.second.as<T>();
}

template<> inline XValue& XObject::get<XValue>( std::string const& name ) {
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name){
		throw logging::Exception(__FILE__, __LINE__, "Object: \"%s\" not found.", name.c_str());
	}
	return p.second;
}

template<typename T> typename _GetType<T>::type XObject::opt(std::string const& name)
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
template<typename T> T const& XObject::set(std::string const& name, T const& obj)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name) {
		it = this->map_.insert(it, std::pair<std::string, XValue>(name, XValue(obj)));
	}else{
		p.second = XValue(obj);
	}
	return obj;
}
template<> inline XValue const& XObject::set<XValue>(std::string const& name, XValue const& obj)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name) {
		it = this->map_.insert(it, std::pair<std::string, XValue>(name, obj));
		return it->second;
	}else{
		return p.second = obj;
	}
}
#define FUNCT(TYPE, VAL) \
		template <> inline bool XValue::is<XValue::TYPE>() const noexcept {\
			return this->type_ == XValue::Type::TYPE##T;\
		};\
		template <> inline typename _GetType<XValue::TYPE>::type XValue::as<XValue::TYPE>() {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				throw logging::Exception(__FILE__, __LINE__, "Type mismatch required: %s actual: %s.", typeid(XValue::TYPE).name(), this->toString().c_str());\
			}\
			return VAL;\
		};\
		template <> inline typename _GetType<XValue::TYPE>::const_type XValue::as<XValue::TYPE>() const {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				throw logging::Exception(__FILE__, __LINE__, "Type mismatch required: %s actual: %s.", typeid(XValue::TYPE).name(), this->toString().c_str());\
			}\
			return VAL;\
		};
FUNCT(Null, spirit_.null_);
FUNCT(Array, *spirit_.array_);
FUNCT(Object, *spirit_.object_);
FUNCT(String, *spirit_.str_);
FUNCT(UInt, spirit_.uint_);
FUNCT(SInt, spirit_.int_);
FUNCT(Float, spirit_.float_);
FUNCT(Bool, spirit_.bool_);

#undef FUNCT

template <> inline XValue XValue::decode<XValue::String>(std::string const& str)
{
	return XValue((String)str);
}
template <> inline XValue XValue::decode<XValue::UInt>(std::string const& str)
{
	bool success = false;
	XValue::UInt val = util::parseInt(str, 0, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to UInt.", str.c_str());
	}
	return XValue((UInt)val);
}
template <> inline XValue XValue::decode<XValue::SInt>(std::string const& str)
{
	bool success = false;
	XValue::SInt val = util::parseInt(str, 0, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to SInt.", str.c_str());
	}
	return XValue((SInt)val);
}
template <> inline XValue XValue::decode<XValue::Float>(std::string const& str)
{
	bool success = false;
	XValue::Float val = util::parseFloat(str, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to Double.", str.c_str());
	}
	return XValue((Float)val);
}
template <> inline XValue XValue::decode<XValue::Bool>(std::string const& str)
{
	bool success = false;
	XValue::SInt val = util::parseBool(str, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to bool", str.c_str());
	}
	return XValue((Bool)val);
}

}}
