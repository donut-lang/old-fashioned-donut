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
	if(std::string("xarray") != elm->Name()){
		throw logging::Exception(__FILE__, __LINE__, "Element is not xarray. type: %s value: %s", elm->Name(), elm->GetText());
	}
	for( tinyxml2::XMLElement* e = elm->FirstChildElement(); e; e = e->NextSiblingElement() ){
		append( XValue::fromXML(e) );
	}
}

tinyxml2::XMLElement* XArray::toXML( tinyxml2::XMLDocument* doc )
{
	tinyxml2::XMLElement* top = doc->NewElement("xarray");
	for(XValue& x : this->list_){
		top->InsertEndChild(x.toXML(doc));
	}
	return top;
}

XObject::XObject( tinyxml2::XMLElement* elm )
{
	accumlate(elm);
}

void XObject::accumlate( tinyxml2::XMLElement* elm )
{
	if(std::string("xobject") != elm->Name()){
		throw logging::Exception(__FILE__, __LINE__, "Element is not xobject. type: %s value: %s", elm->Name(), elm->GetText());
	}
	for( tinyxml2::XMLElement* e = elm->FirstChildElement(); e; e = e->NextSiblingElement() ){
		const char* const name = e->Attribute("name");
		if(!name){
			throw logging::Exception(__FILE__, __LINE__, "Object element name not specified. type: %s value: %s", e->Name(), e->GetText());
		}
		set(name, XValue::fromXML(e));
	}
}

tinyxml2::XMLElement* XObject::toXML( tinyxml2::XMLDocument* doc )
{
	tinyxml2::XMLElement* top = doc->NewElement("xobject");
	for(std::pair<std::string const, XValue>& x : this->map_){
		tinyxml2::XMLElement* e = x.second.toXML(doc);
		e->SetAttribute("name", x.first.c_str());
		top->InsertEndChild(e);
	}
	return top;
}

XValue XValue::fromXML( tinyxml2::XMLElement* elm ) {
	std::string const name(elm->Name());
	if(name=="xnull"){
		return XValue();
	}else if( name == "xarray" ){
		return XValue(Handler<XArray>( new XArray(elm)));
	}else if(name=="xobject") {
		return XValue(Handler<XObject>( new XObject(elm)) );
	}else if(name=="xstring") {
		return XValue::decode<String>( elm->GetText() );
	}else if(name=="xuint") {
		return XValue::decode<UInt>( elm->GetText() );
	}else if(name=="xsint") {
		return XValue::decode<SInt>( elm->GetText() );
	}else if(name=="xbool") {
		return XValue::decode<Bool>( elm->GetText() );
	}else if(name=="xfloat") {
		return XValue::decode<Float>( elm->GetText() );
	}else{
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon type!: %s", name.c_str());
	}
}

tinyxml2::XMLElement* XValue::toXML( tinyxml2::XMLDocument* doc )
{
	switch(type_){
	case NullT:
		return doc->NewElement("xnull");
	case ArrayT:
		return (*this->spirit_.array_)->toXML(doc);
	case ObjectT:
		return (*this->spirit_.object_)->toXML(doc);
	case StringT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xstring");
		elm->InsertEndChild(doc->NewText(this->spirit_.str_->c_str()));
		return elm;
	}
	case UIntT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xuint");
		elm->InsertEndChild(doc->NewText(util::toString(this->spirit_.uint_).c_str()));
		return elm;
	}
	case SIntT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xsint");
		elm->InsertEndChild(doc->NewText(util::toString(this->spirit_.int_).c_str()));
		return elm;
	}
	case FloatT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xfloat");
		elm->InsertEndChild(doc->NewText(util::toString(this->spirit_.float_).c_str()));
		return elm;
	}
	case BoolT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xbool");
		elm->InsertEndChild(doc->NewText(util::toString(this->spirit_.bool_).c_str()));
		return elm;
	}
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon type!: %d", type_);
	}
}

std::string XValue::toString() const noexcept {
	switch ( this->type_ ){
	case Type::NullT:
		return "Null";
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
		return util::toString(this->spirit_.bool_);
	case Type::FloatT:
		return util::toString(this->spirit_.float_);
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon type!: %d", type_);
	}
}

void XValue::remove()
{
	switch(this->type_){
	case Type::ArrayT:
		delete this->spirit_.array_;
		this->spirit_.array_ = nullptr;
		break;
	case Type::ObjectT:
		delete this->spirit_.object_;
		this->spirit_.object_ = nullptr;
		break;
	case Type::StringT:
		delete this->spirit_.str_;
		this->spirit_.str_ = nullptr;
		break;
	default:
		break;
	}
}
void XValue::grab(XValue&& o)
{
	o.type_ = NullT;
	switch(type_){
	case NullT:
		break;
	case ArrayT:
		this->spirit_.array_ = o.spirit_.array_;
		o.spirit_.array_ = nullptr;
		break;
	case ObjectT:
		this->spirit_.object_ = o.spirit_.object_;
		o.spirit_.object_ = nullptr;
		break;
	case StringT:
		this->spirit_.str_ = o.spirit_.str_;
		o.spirit_.str_ = nullptr;
		break;
	case UIntT:
		this->spirit_.uint_ = o.spirit_.uint_;
		break;
	case SIntT:
		this->spirit_.int_ = o.spirit_.int_;
		break;
	case FloatT:
		this->spirit_.float_ = o.spirit_.float_;
		break;
	case BoolT:
		this->spirit_.bool_ = o.spirit_.bool_;
		break;
	}
}
void XValue::copy(XValue const& o)
{
	switch(type_){
	case NullT:
		break;
	case ArrayT:
		this->spirit_.array_ = new Handler<Array>(*o.spirit_.array_);
		break;
	case ObjectT:
		this->spirit_.object_ = new Handler<Object>(*o.spirit_.object_);
		break;
	case StringT:
		this->spirit_.str_ = new std::string(*o.spirit_.str_);
		break;
	case UIntT:
		this->spirit_.uint_ = o.spirit_.uint_;
		break;
	case SIntT:
		this->spirit_.int_ = o.spirit_.int_;
		break;
	case FloatT:
		this->spirit_.float_ = o.spirit_.float_;
		break;
	case BoolT:
		this->spirit_.bool_ = o.spirit_.bool_;
		break;
	}
}

void XValue::swap( XValue& o )
{
	std::swap(o.type_, type_);
	std::swap(o.spirit_, spirit_);
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
