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
#include <vector>
#include <cstddef>
#include <type_traits>
#include "Common.h"
#include "Handler.h"
#include "ClassUtil.h"
#include "PairFunctor.h"

namespace tinyxml2 {
class XMLDocument;
class XMLElement;
}

namespace tarte {

class XArray;
class XObject;

template <typename T, typename U=void> struct _TypeAdapter;

class XValue {
	STACK_OBJECT(XValue);
public: /* data type decl */
	enum Type {
		NullT,
		UIntT,
		SIntT,
		FloatT,
		BoolT,
		StringT,
		BinaryT,
		ArrayT,
		ObjectT,
	};
	typedef std::nullptr_t Null;
	typedef std::string String;
	typedef XArray Array;
	typedef std::vector<char> Binary;
	typedef XObject Object;
	typedef unsigned int UInt;
	typedef signed int SInt;
	typedef float Float;
	typedef bool Bool;
private: /* internal data operations */
	enum Type type_;
	union {
		Null null_;
		Handler<Array>* array_;
		Handler<Object>* object_;
		Binary* binary_;
		String* str_;
		UInt uint_;
		SInt int_;
		Float float_;
		Bool bool_;
	} spirit_;
	void remove();
	void grab(XValue&& o);
	void copy(XValue const& o);
public: /* copy/assign */
	XValue(XValue const& o):type_(o.type_){ this->copy(o); }
	XValue(XValue&& o):type_(o.type_){ this->grab(std::move(o)); }
	XValue& operator=(XValue const& o){
		this->remove();
		this->type_ = o.type_;
		this->copy(o);
		return *this;
	}
	XValue& operator=(XValue&& o){
		this->remove();
		this->type_ = o.type_;
		this->grab(std::move(o));
		return *this;
	}
public: /* constructors */
	// default => null
	explicit inline XValue():type_(Type::NullT){};
	//literal types
	explicit inline XValue(std::nullptr_t const& val):type_(Type::NullT){ this->spirit_.null_ = val; };
	explicit inline XValue(UInt const& val):type_(Type::UIntT){ this->spirit_.uint_=val; };
	explicit inline XValue(SInt const& val):type_(Type::SIntT){ this->spirit_.int_=val; };
	explicit inline XValue(Float const& val):type_(Type::FloatT){ this->spirit_.float_=val; };
	explicit inline XValue(Bool const& val):type_(Type::BoolT){ this->spirit_.bool_=val; };

	//string
	explicit inline XValue(String const& val):type_(Type::StringT){ this->spirit_.str_= new std::string(val); };
	explicit inline XValue(const char* const& val):type_(Type::StringT){ this->spirit_.str_ = new std::string(val); };

	//binary
	explicit inline XValue(const char* const& val, std::size_t len):type_(Type::BinaryT){ this->spirit_.binary_ = new Binary(val, val+len); };
	explicit inline XValue(const unsigned char* const& val, std::size_t len):type_(Type::BinaryT){ this->spirit_.binary_ = new Binary(reinterpret_cast<char const*>(val), reinterpret_cast<char const*>(val+len)); };
	explicit inline XValue(const signed char* const& val, std::size_t len):type_(Type::BinaryT){ this->spirit_.binary_ = new Binary(reinterpret_cast<char const*>(val), reinterpret_cast<char const*>(val+len)); };

	explicit inline XValue(Binary const& val):type_(Type::BinaryT){ this->spirit_.binary_ = new std::vector<char>(val); };
	explicit inline XValue(std::vector<unsigned char> const& val):type_(Type::BinaryT){ this->spirit_.binary_ = new Binary(reinterpret_cast<char const*>(&val[0]), reinterpret_cast<char const*>(&val[val.size()])); };
	explicit inline XValue(std::vector<signed char> const& val):type_(Type::BinaryT){ this->spirit_.binary_ = new Binary(reinterpret_cast<char const*>(&val[0]), reinterpret_cast<char const*>(&val[val.size()])); };

	// array, obj
	inline XValue(Handler<XArray> const& val):type_(Type::ArrayT){ this->spirit_.array_= new Handler<Array>( val ); };
	inline XValue(Handler<XObject> const& val):type_(Type::ObjectT){ this->spirit_.object_= new Handler<Object>( val ); };

	template <typename T> explicit inline XValue(T const& val);

	~XValue() noexcept { this->remove(); }
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T> bool is() const noexcept;
	template <typename T> typename _TypeAdapter<T>::return_type as();
	template <typename T> typename _TypeAdapter<T>::return_const_type as() const;
	template <typename T> typename _TypeAdapter<T>::return_type as(T& v);
	template <typename T> typename _TypeAdapter<T>::return_const_type as(T& v) const;
	std::string typeString() const noexcept;
	void swap( XValue& o );
private:
	template <typename T> void init(T const& val);
	template <typename T> bool isImpl() const noexcept;
	template <typename T> typename _TypeAdapter<T>::return_type asImpl();
	template <typename T> typename _TypeAdapter<T>::return_const_type asImpl() const;
public:
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	static XValue fromXML( tinyxml2::XMLElement* elm );
public:
	template <typename T> static XValue fromString( std::string const& str );
};

class XArray : public HandlerBody<XArray> {
private:
	std::vector<XValue> list_;
public:
	XArray( tinyxml2::XMLElement* elm );
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	XArray(){};
	~XArray() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T> bool has( std::size_t const& idx ) const;
	template <typename T> typename _TypeAdapter<T>::return_type get( std::size_t const& idx );
	template <typename T> typename _TypeAdapter<T>::return_type get( std::size_t const& idx, T& v );
	template <typename T> typename _TypeAdapter<T>::return_type set( std::size_t const& idx, T const& obj );
	template <typename T> typename _TypeAdapter<T>::return_type append( T const& obj );
public:
	std::vector<XValue>::iterator begin(){ return this->list_.begin(); };
	std::vector<XValue>::iterator end(){ return this->list_.end(); };
	std::vector<XValue>::const_iterator cbegin(){ return this->list_.cbegin(); };
	std::vector<XValue>::const_iterator cend(){ return this->list_.cend(); };
	std::vector<XValue>::size_type size(){ return this->list_.size(); };
};

class XObject : public HandlerBody<XObject> {
	typedef PairCompare<std::string, XValue> Comparator;
	std::vector<std::pair<std::string, XValue> > map_;
public:
	XObject( tinyxml2::XMLElement* elm );
	tinyxml2::XMLElement* toXML( tinyxml2::XMLDocument* doc );
	void accumlate( tinyxml2::XMLElement* elm );
	XObject() {}
	~XObject() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T> bool has( std::string const& name );
	template <typename T> typename _TypeAdapter<T>::return_type get( std::string const& name );
	template <typename T> typename _TypeAdapter<T>::return_type get( std::string const& name, T& v );
	template <typename T> typename _TypeAdapter<T>::return_type set( std::string const& name, T const& obj );
public:
	std::vector<std::pair<std::string, XValue> >::iterator begin(){ return this->map_.begin(); };
	std::vector<std::pair<std::string, XValue> >::iterator end(){ return this->map_.end(); };
	std::vector<std::pair<std::string, XValue> >::const_iterator cbegin(){ return this->map_.cbegin(); };
	std::vector<std::pair<std::string, XValue> >::const_iterator cend(){ return this->map_.cend(); };
	std::vector<std::pair<std::string, XValue> >::size_type size(){ return this->map_.size(); };
};

inline void swap(XValue& a, XValue& b) {
	a.swap(b);
}

typedef XValue::Null XNull;
typedef XValue::String XString;
typedef XValue::UInt XUInt;
typedef XValue::SInt XSInt;
typedef XValue::Float XFloat;
typedef XValue::Bool XBool;
typedef XValue::Binary XBinary;

}

#include "internal/XVal.h"
