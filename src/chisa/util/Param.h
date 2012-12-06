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

namespace xvalue {
class XArray;
class XObject;

template <typename T>
struct _GetType{
	typedef T& type;
	typedef T const& const_type;
};
template <>
struct _GetType<XArray> {
	typedef Handler<XArray> type;
	typedef Handler<XArray> const_type;
};
template <>
struct _GetType<XObject> {
	typedef Handler<XObject> type;
	typedef Handler<XObject> const_type;
};

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
	Handler<Array> array_;
	Handler<Object> object_;
	union {
		String* str_;
		UInt uint_;
		SInt int_;
		Float dbl_;
		Bool b_;
	} spirit_;
public:
	XValue(XValue const& o):type_(o.type_){
		switch(type_){
		case NullT:
			break;
		case ArrayT:
			this->array_ = o.array_;
			break;
		case ObjectT:
			this->object_ = o.object_;
			break;
		case StringT:
			this->spirit_.str_ = new std::string(*o.spirit_.str_);
			break;
		case UIntT:
			this->spirit_ = o.spirit_;
			break;
		case SIntT:
			this->spirit_ = o.spirit_;
			break;
		case FloatT:
			this->spirit_ = o.spirit_;
			break;
		case BoolT:
			this->spirit_ = o.spirit_;
			break;
		}
	}
	XValue& operator=(XValue const& o){
		this->type_ = o.type_;
		switch(type_){
		case NullT:
			break;
		case ArrayT:
			this->array_ = o.array_;
			break;
		case ObjectT:
			this->object_ = o.object_;
			break;
		case StringT:
			this->spirit_.str_ = new std::string(*o.spirit_.str_);
			break;
		case UIntT:
			this->spirit_ = o.spirit_;
			break;
		case SIntT:
			this->spirit_ = o.spirit_;
			break;
		case FloatT:
			this->spirit_ = o.spirit_;
			break;
		case BoolT:
			this->spirit_ = o.spirit_;
			break;
		}
		return *this;
	}
	explicit inline XValue():type_(Type::NullT){};
	explicit inline XValue(Float const& val):type_(Type::FloatT){ this->spirit_.dbl_=val; };
	explicit inline XValue(Bool const& val):type_(Type::BoolT){ this->spirit_.b_=val; };
	explicit inline XValue(SInt const& val):type_(Type::SIntT){ this->spirit_.int_=val; };
	explicit inline XValue(UInt const& val):type_(Type::UIntT){ this->spirit_.uint_=val; };
	explicit inline XValue(Handler<XArray> const& val):type_(Type::ArrayT){ this->array_=val; };
	explicit inline XValue(Handler<XObject> const& val):type_(Type::ObjectT){ this->object_=val; };
	explicit inline XValue(String const& val):type_(Type::StringT){ this->spirit_.str_= new std::string(val); };
	explicit inline XValue(const char* const& val):type_(Type::StringT){ this->spirit_.str_ = new std::string(val); };

	inline bool onFree() const noexcept { return false; };
	~XValue() noexcept {
		switch(this->type_){
		case Type::StringT:
			delete this->spirit_.str_;
			break;
		default:
			break;
		}
	}
	template <typename T> bool is() const noexcept;
	template <typename T> typename _GetType<T>::type get();
	template <typename T> typename _GetType<T>::const_type get() const;
	template <typename T> static XValue fromString( std::string const& str );

	static XValue fromXML( tinyxml2::XMLElement* elm );
	std::string toString() const noexcept;
};

class XArray : public HandlerBody<XArray> {
private:
	std::vector<XValue > list_;
public:
	XArray( tinyxml2::XMLElement* elm );
	XArray(){};
	~XArray() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	std::size_t size() const noexcept { return this->list_.size(); };
	template <typename T>
	T& get( std::size_t const& idx ) {
		if( idx < 0 || idx >= list_.size() ){
			throw logging::Exception(__FILE__, __LINE__, "idx: %d >= size: %d ", idx, list_.size());
		}
		XValue& val = list_[idx];
		if( !val.is<T>() ) {
			throw logging::Exception(__FILE__, __LINE__, "Object: %s type mismatch %s != %s.", typeid(T).name(), val.toString().c_str());
		}
		return val.get<T>();
	}
	template <typename T>
	T const& set( const std::size_t& idx, T const& obj ) {
		if( idx < 0 || idx >= list_.size() ){
			throw logging::Exception(__FILE__, __LINE__, "idx: %d >= size: %d ", idx, list_.size());
		}
		return list_[idx] = XValue(obj);
	}
	template <typename T>
	T const& append( T const& obj ) {
		this->list_.push_back(XValue(obj));
		return obj;
	}
};

class XObject : public HandlerBody<XObject> {
	std::unordered_map<std::string, XValue> map_;
public:
	XObject( tinyxml2::XMLElement* elm );
	XObject() {}
	~XObject() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	template <typename T>
	typename _GetType<T>::type get( std::string const& name ) {
		std::unordered_map<std::string, XValue>::iterator it = this->map_.find(name);
		if( it == this->map_.end()){
			throw logging::Exception(__FILE__, __LINE__, "Object: %s not found.", name.c_str());
		}
		if( !it->second.is<T>() ) {
			throw logging::Exception(__FILE__, __LINE__, "Object: %s type mismatch %s != %s.", typeid(T).name(), it->second.toString().c_str());
		}
		return it->second.get<T>();
	}
	template <typename T>
	typename _GetType<T>::type opt( std::string const& name ) {
		std::unordered_map<std::string, XValue>::iterator it = this->map_.find(name);
		if( it == this->map_.end() || !it->second.is<T>() ){
			return T();
		}
		return it->second.get<T>();
	}
	template <typename T>
	bool has( std::string const& name ) {
		std::unordered_map<std::string, XValue>::iterator it = this->map_.find(name);
		if( it == this->map_.end() || !it->second.is<T>() ){
			return false;
		}
		return true;
	}
	template <typename T>
	T const& set( const std::string& name, T const& obj ) {
		return this->map_[name] = XValue(obj);
	}
};

#define FUNCT(TYPE, VAL) \
		template <> inline bool XValue::is<XValue::TYPE>() const noexcept {\
			return this->type_ == XValue::Type::TYPE##T;\
		};\
		template <> inline typename _GetType<XValue::TYPE>::type XValue::get<XValue::TYPE>() {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				throw logging::Exception(__FILE__, __LINE__, "Type mismatch %s != %s.", typeid(XValue::TYPE).name(), this->toString().c_str());\
			}\
			return VAL;\
		};\
		template <> inline typename _GetType<XValue::TYPE>::const_type XValue::get<XValue::TYPE>() const {\
			if(this->type_ != XValue::Type::TYPE##T) {\
				throw logging::Exception(__FILE__, __LINE__, "Type mismatch %s != %s.", typeid(XValue::TYPE).name(), this->toString().c_str());\
			}\
			return VAL;\
		};
FUNCT(Array, array_);
FUNCT(Object, object_);
FUNCT(String, *spirit_.str_);
FUNCT(UInt, spirit_.uint_);
FUNCT(SInt, spirit_.int_);
FUNCT(Float, spirit_.dbl_);
FUNCT(Bool, spirit_.b_);

#undef FUNCT

template <> inline XValue XValue::fromString<XValue::String>(std::string const& str)
{
	return XValue((String)str);
}
template <> inline XValue XValue::fromString<XValue::UInt>(std::string const& str)
{
	bool success = false;
	XValue::UInt val = util::parseInt(str, 0, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to UInt.", str.c_str());
	}
	return XValue((UInt)val);
}
template <> inline XValue XValue::fromString<XValue::SInt>(std::string const& str)
{
	bool success = false;
	XValue::SInt val = util::parseInt(str, 0, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to SInt.", str.c_str());
	}
	return XValue((SInt)val);
}
template <> inline XValue XValue::fromString<XValue::Float>(std::string const& str)
{
	bool success = false;
	XValue::Float val = util::parseFloat(str, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to Double.", str.c_str());
	}
	return XValue((Float)val);
}
template <> inline XValue XValue::fromString<XValue::Bool>(std::string const& str)
{
	bool success = false;
	XValue::SInt val = util::parseBool(str, &success);
	if(!success){
		throw logging::Exception(__FILE__, __LINE__, "Failed to convert %s to bool", str.c_str());
	}
	return XValue((Bool)val);
}

}

typedef xvalue::XValue XValue;
typedef xvalue::XArray XArray;
typedef xvalue::XObject XObject;
typedef XValue::String XString;
typedef XValue::UInt XUInt;
typedef XValue::SInt XSInt;
typedef XValue::Float XFloat;
typedef XValue::Bool XBool;

class Param {
	DISABLE_COPY_AND_ASSIGN(Param);
public:
	static constexpr const char* ElemName="param";
	struct AttrName{
		static constexpr const char* Name = "name";
		static constexpr const char* Type = "type";
	};
	struct TypeName {
		static constexpr const char* Integer = "int";
		static constexpr const char* String = "string";
		static constexpr const char* Float = "float";
		static constexpr const char* Boolean = "boolean";
		static constexpr const char* Bool = "bool";
	};
private:
	const std::string name_;
public:
	Param(const std::string& name):name_(name){};
	virtual ~Param(){};
public:
	std::string name() const{ return name_; };
public:
	virtual bool queryInt(int* val) { return false; };
	virtual bool queryString(std::string* val) { return false; };
	virtual bool queryFloat(float* val) { return false; };
	virtual bool queryBool(bool* val) { return false; };
	virtual tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc) = 0;
public:
	static std::shared_ptr<Param> createParam(const std::string& name, const std::string& type, const std::string& value);
public:
	static std::shared_ptr<Param> parseTree(tinyxml2::XMLElement* elem);
};

class ParamSet {
	std::map<std::string, std::shared_ptr<Param> > params_;
public:
	ParamSet(){};
	virtual ~ParamSet() noexcept = default;
public:
	void add(const std::string& name, const std::string& type, const std::string& value);
	void add(std::shared_ptr<Param> p);
	std::shared_ptr<Param> get(const std::string& name);
	bool has(const std::string& name);
	std::size_t size() const{ return this->params_.size(); };
	bool queryInt(const std::string& name, int* val);
//	bool queryInt(const std::string& name, int64_t* val) = delete;
//	bool queryInt(const std::string& name, uint64_t* val) = delete;
	bool queryString(const std::string& name, std::string* val);
	bool queryFloat(const std::string& name, float* val);
	bool queryBool(const std::string& name, bool* val);

	void addInt(const std::string& name, int value);
//	void addInt(const std::string& name, int64_t value) = delete;
//	void addInt(const std::string& name, uint64_t value) = delete;
	void addString(const std::string& name, const std::string value);
	void addFloat(const std::string& name, float value);
	void addBool(const std::string& name, bool value);

	int getInt(const std::string& name);
	std::string getString(const std::string& name);
	float getFloat(const std::string& name);
	bool getBool(const std::string& name);
public:
	void parseTree(tinyxml2::XMLElement* elem);
	tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc);
};

}}
