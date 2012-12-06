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

#include "Param.h"
#include <cstdlib>
#include <algorithm>
#include "../util/StringUtil.h"
#include "../logging/Exception.h"

namespace chisa {
namespace util {
namespace xvalue {

XArray::XArray( tinyxml2::XMLElement* elm )
{
	for( tinyxml2::XMLElement* e = elm->FirstChildElement(); e; e = e->NextSiblingElement() ){
		append( XValue::fromXML(e) );
	}
}

XObject::XObject( tinyxml2::XMLElement* elm )
{
	for( tinyxml2::XMLElement* e = elm->FirstChildElement("xpair"); e; e = e->NextSiblingElement("xpair") ){
		set(e->Attribute("name"), XValue::fromXML(e->FirstChildElement()));
	}
}

XValue XValue::fromXML( tinyxml2::XMLElement* elm ) {
	std::string const name(elm->Name());
	if( name == "xarray" ){
		return XValue(Handler<XArray>( new XArray(elm)));
	}else if(name=="xobject") {
		return XValue(Handler<XObject>( new XObject(elm)) );
	}else if(name=="xstring") {
		return XValue::fromString<String>( elm->GetText() );
	}else if(name=="xuint") {
		return XValue::fromString<UInt>( elm->GetText() );
	}else if(name=="xsint") {
		return XValue::fromString<SInt>( elm->GetText() );
	}else if(name=="xbool") {
		return XValue::fromString<Bool>( elm->GetText() );
	}else if(name=="xfloat") {
		return XValue::fromString<Float>( elm->GetText() );
	}else{
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon type!: %s", name.c_str());
	}
}
std::string XValue::toString() const noexcept {
	switch ( this->type_ ){
	case Type::ArrayT:
		return "Array";
	case Type::ObjectT:
		return "Object";
	case Type::StringT:
		return *this->spirit_.str_;
	case Type::UIntT:
		return util::toString( this->spirit_.uint_ );
	case Type::SIntT:
		return util::toString( this->spirit_.int_ );
	case Type::BoolT:
		return util::toString(this->spirit_.b_);
	case Type::FloatT:
		return util::toString(this->spirit_.dbl_);
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon type!: %d", type_);
	}
}

}

using namespace tinyxml2;

template<class T>
T* constructParam(std::string& name, std::string& value){
	return new T(name, value);
}

class StringParam : public Param {
private:
	const std::string value_;
public:
	StringParam(const std::string& name, const std::string& value)
	:Param(name)
	,value_(value)
	{}
	virtual ~StringParam(){}
	virtual bool queryString(std::string* val) override
	{
		*val = value_;
		return true;
	}
	virtual tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc) override
	{
		tinyxml2::XMLElement* elm = doc->NewElement(Param::ElemName);
		elm->SetAttribute(AttrName::Name, this->name().c_str());
		elm->SetAttribute(AttrName::Type, TypeName::String);
		elm->InsertFirstChild(doc->NewText(this->value_.c_str()));
		return elm;
	}
};

class IntegerParam : public Param {
private:
	int value_;
	bool succeed_;
public:
	IntegerParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		this->value_ = parseInt(value, 0, &this->succeed_);
	}
	IntegerParam(const std::string& name, const int value)
	:Param(name)
	,value_(value)
	,succeed_(true)
	{
	}
	virtual bool queryInt(int* val) override
	{
		if(!succeed_){
			return false;
		}
		*val = value_;
		return true;
	}
	virtual tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc) override
	{
		if(!succeed_){
			return nullptr;
		}
		tinyxml2::XMLElement* elm = doc->NewElement(Param::ElemName);
		elm->SetAttribute(AttrName::Name, this->name().c_str());
		elm->SetAttribute(AttrName::Type, TypeName::Integer);
		elm->InsertFirstChild( doc->NewText(util::toString(this->value_).c_str()) );
		return elm;
	}
};

class FloatParam : public Param {
private:
	float value_;
	bool succeed_;
public:
	FloatParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		this->value_ = parseFloat(value, &this->succeed_);
	}
	FloatParam(const std::string& name, const float value)
	:Param(name)
	,value_(value)
	,succeed_(true)
	{
	}
	virtual ~FloatParam(){}
	virtual bool queryFloat(float* val) override
	{
		if(!succeed_){
			return false;
		}
		*val = this->value_;
		return true;
	}
	virtual tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc) override
	{
		if(!succeed_){
			return nullptr;
		}
		tinyxml2::XMLElement* elm = doc->NewElement(Param::ElemName);
		elm->SetAttribute(AttrName::Name, this->name().c_str());
		elm->SetAttribute(AttrName::Type, TypeName::Float);
		elm->InsertFirstChild( doc->NewText(util::toString(this->value_).c_str()) );
		return elm;
	}
};

class BoolParam : public Param {
private:
	bool value_;
	bool succeed_;
public:
	BoolParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		this->value_ = parseBool(value, &this->succeed_);
	}
	BoolParam(const std::string& name, const bool value)
	:Param(name)
	,value_(value)
	,succeed_(true)
	{
	}
	virtual ~BoolParam(){}
	virtual bool queryBool(bool* val) override
	{
		if(!succeed_){
			return false;
		}
		*val = this->value_;
		return true;
	}
	virtual tinyxml2::XMLElement* synthTree(tinyxml2::XMLDocument* doc) override
	{
		if(!succeed_){
			return nullptr;
		}
		tinyxml2::XMLElement* elm = doc->NewElement(Param::ElemName);
		elm->SetAttribute(AttrName::Name, this->name().c_str());
		elm->SetAttribute(AttrName::Type, TypeName::Boolean);
		elm->InsertFirstChild( doc->NewText(util::toString(this->value_).c_str()) );
		return elm;
	}
};
std::shared_ptr<Param> Param::createParam(const std::string& name, const std::string& type, const std::string& value)
{
	if(type==TypeName::String){
		return std::shared_ptr<Param>(new StringParam(name, value));
	}else if(type==TypeName::Integer){
		return std::shared_ptr<Param>(new IntegerParam(name, value));
	}else if(type==TypeName::Float){
		return std::shared_ptr<Param>(new FloatParam(name, value));
	}else if(type==TypeName::Boolean || type==TypeName::Bool){
		return std::shared_ptr<Param>(new BoolParam(name, value));
	}else{
		return std::shared_ptr<Param>(new StringParam(name, value));
	}
}

void ParamSet::add(std::shared_ptr<Param> p)
{
	this->params_.insert( std::make_pair(p->name(), p) );
}
void ParamSet::add(const std::string& name, const std::string& type, const std::string& value)
{
	std::shared_ptr<Param> p = Param::createParam(name, type, value);
	this->params_.insert(std::make_pair(name, p));
}
std::shared_ptr<Param> ParamSet::get(const std::string& name)
{
	auto p = this->params_.find(name);
	if(p == this->params_.end()){
		return std::shared_ptr<Param>();
	}
	return p->second;
}

bool ParamSet::has(const std::string& name)
{
	return this->params_.find(name) != this->params_.end();
}

std::shared_ptr<Param> Param::parseTree(tinyxml2::XMLElement* elem)
{
	std::string elemName(elem->Name());
	const char* name;
	const char* type;
	const char* value;
	if(elemName != Param::ElemName || !(name = elem->Attribute(AttrName::Name, nullptr)) || !(value = elem->GetText()) ){
		return std::shared_ptr<Param>();
	}
	if( !(type = elem->Attribute(AttrName::Type, nullptr) ) ){
		type=TypeName::String;
	}
	elem->FirstChildElement();
	return Param::createParam(name, type, value);
}

void ParamSet::parseTree(tinyxml2::XMLElement* elem)
{
	for(XMLNode* it=elem->FirstChild(); it; it=it->NextSibling()){
		if(XMLElement* paramElem = it->ToElement()){
			std::shared_ptr<Param> p = Param::parseTree(paramElem);
			if(p){
				this->add(p);
			}
		}
	}
}

void ParamSet::addInt(const std::string& name, int value)
{
	this->params_.insert( std::pair<std::string, std::shared_ptr<Param> >(name, std::shared_ptr<Param>( new IntegerParam(name, value)) ) );
}

void ParamSet::addString(const std::string& name, const std::string value)
{
	this->params_.insert( std::pair<std::string, std::shared_ptr<Param> >(name, std::shared_ptr<Param>( new StringParam(name, value)) ) );
}
void ParamSet::addFloat(const std::string& name, float value)
{
	this->params_.insert( std::pair<std::string, std::shared_ptr<Param> >(name, std::shared_ptr<Param>( new FloatParam(name, value)) ) );
}
void ParamSet::addBool(const std::string& name, bool value)
{
	this->params_.insert( std::pair<std::string, std::shared_ptr<Param> >(name, std::shared_ptr<Param>( new BoolParam(name, value)) ) );
}


tinyxml2::XMLElement* ParamSet::synthTree(tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* elm = doc->NewElement(Param::ElemName);
	for(auto it : this->params_){
		auto param = it.second;
		if(auto child = param->synthTree(doc)){
			elm->InsertEndChild(child);
		}
	}
	return elm;
}

bool ParamSet::queryInt(const std::string& name, int* val)
{
	if(std::shared_ptr<Param> p = this->get(name)){
		return p->queryInt(val);
	}
	return false;
}
bool ParamSet::queryString(const std::string& name, std::string* val)
{
	if(std::shared_ptr<Param> p = this->get(name)){
		return p->queryString(val);
	}
	return false;
}
bool ParamSet::queryFloat(const std::string& name, float* val)
{
	if(std::shared_ptr<Param> p = this->get(name)){
		return p->queryFloat(val);
	}
	return false;
}

bool ParamSet::queryBool(const std::string& name, bool* val)
{
	if(std::shared_ptr<Param> p = this->get(name)){
		return p->queryBool(val);
	}
	return false;
}

int ParamSet::getInt(const std::string& name)
{
	int val;
	if(!this->has(name)){
		throw logging::Exception(__FILE__, __LINE__, "Does not have parameter: %s", name.c_str());
	}
	if(! this->queryInt(name, &val) ){
		throw logging::Exception(__FILE__, __LINE__, "Failed to query int: \"%s\"", name.c_str());
	}
	return val;
}
std::string ParamSet::getString(const std::string& name)
{
	std::string val;
	if(!this->has(name)){
		throw logging::Exception(__FILE__, __LINE__, "Does not have parameter: %s", name.c_str());
	}
	if(! this->queryString(name, &val) ){
		throw logging::Exception(__FILE__, __LINE__, "Failed to query string: \"%s\"", name.c_str());
	}
	return val;
}
float ParamSet::getFloat(const std::string& name)
{
	float val;
	if(!this->has(name)){
		throw logging::Exception(__FILE__, __LINE__, "Does not have parameter: %s", name.c_str());
	}
	if(! this->queryFloat(name, &val) ){
		throw logging::Exception(__FILE__, __LINE__, "Failed to query float: \"%s\"", name.c_str());
	}
	return val;
}

bool ParamSet::getBool(const std::string& name)
{
	bool val;
	if(!this->has(name)){
		throw logging::Exception(__FILE__, __LINE__, "Does not have parameter: %s", name.c_str());
	}
	if(! this->queryBool(name, &val) ){
		throw logging::Exception(__FILE__, __LINE__, "Failed to query bool: \"%s\"", name.c_str());
	}
	return val;
}

}}
