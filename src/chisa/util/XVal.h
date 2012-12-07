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

#include <tinyxml2.h>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "ClassUtil.h"
#include "StringUtil.h"
#include "../Handler.h"

namespace chisa {
namespace util {

class XArray;
class XObject;

template <typename T> struct _GetType;

class XValue {
	STACK_OBJECT(XValue);
public:
	enum Type {
		NullT,
		ArrayT,
		ObjectT,
		StringT,
		UIntT,
		SIntT,
		FloatT,
		BoolT
	};
	typedef std::nullptr_t Null;
	typedef std::string String;
	typedef XArray Array;
	typedef XObject Object;
	typedef unsigned int UInt;
	typedef signed int SInt;
	typedef float Float;
	typedef bool Bool;
private:
	enum Type type_;
	union {
		Handler<Array>* array_;
		Handler<Object>* object_;
		String* str_;
		UInt uint_;
		SInt int_;
		Float float_;
		Bool bool_;
	} spirit_;
	void remove();
	void grab(XValue&& o);
	void copy(XValue const& o);
public:
	XValue(XValue const& o):type_(o.type_){ this->copy(o); }
	XValue(XValue&& o):type_(o.type_){ this->grab(static_cast<XValue&&>(o)); }
	XValue& operator=(XValue const& o){
		this->remove();
		this->type_ = o.type_;
		this->copy(o);
		return *this;
	}
	XValue& operator=(XValue&& o){
		this->remove();
		this->type_ = o.type_;
		this->grab(static_cast<XValue&&>(o));
		return *this;
	}
	explicit inline XValue():type_(Type::NullT){};
	explicit inline XValue(Float const& val):type_(Type::FloatT){ this->spirit_.float_=val; };
	explicit inline XValue(Bool const& val):type_(Type::BoolT){ this->spirit_.bool_=val; };
	explicit inline XValue(SInt const& val):type_(Type::SIntT){ this->spirit_.int_=val; };
	explicit inline XValue(UInt const& val):type_(Type::UIntT){ this->spirit_.uint_=val; };
	explicit inline XValue(Handler<XArray> const& val):type_(Type::ArrayT){ this->spirit_.array_= new Handler<Array>( val ); };
	explicit inline XValue(Handler<XObject> const& val):type_(Type::ObjectT){ this->spirit_.object_= new Handler<Object>( val ); };
	explicit inline XValue(String const& val):type_(Type::StringT){ this->spirit_.str_= new std::string(val); };
	explicit inline XValue(const char* const& val):type_(Type::StringT){ this->spirit_.str_ = new std::string(val); };
	~XValue() noexcept { this->remove(); }
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T> bool is() const noexcept;
	template <typename T> typename _GetType<T>::type get();
	template <typename T> typename _GetType<T>::const_type get() const;
	template <typename T> static XValue decode( std::string const& str );

	static XValue fromXML( tinyxml2::XMLElement* elm );
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	std::string toString() const noexcept;
	void swap( XValue& o );
};

class XArray : public HandlerBody<XArray> {
private:
	std::vector<XValue > list_;
public:
	XArray( tinyxml2::XMLElement* elm );
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	XArray(){};
	~XArray() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	std::size_t size() const noexcept { return this->list_.size(); };
	template <typename T> T& get( std::size_t const& idx );
	template <typename T> T const& set( const std::size_t& idx, T const& obj );
	template <typename T> T const& append( T const& obj );
};

class XObject : public HandlerBody<XObject> {
	std::unordered_map<std::string, XValue> map_;
public:
	XObject( tinyxml2::XMLElement* elm );
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	void accumlate( tinyxml2::XMLElement* elm );
	XObject() {}
	~XObject() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T> typename _GetType<T>::type get( std::string const& name );
	template <typename T> typename _GetType<T>::type opt( std::string const& name );
	template <typename T> bool has( std::string const& name );
	template <typename T> T const& set( const std::string& name, T const& obj );
};


inline void swap(XValue& a, XValue& b) {
	a.swap(b);
}

typedef XValue::String XString;
typedef XValue::UInt XUInt;
typedef XValue::SInt XSInt;
typedef XValue::Float XFloat;
typedef XValue::Bool XBool;


}}

#include "internal/XVal.h"
