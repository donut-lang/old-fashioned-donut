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
#include "../logging/Exception.h"

namespace chisa {
namespace util {

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
};

class IntegerParam : public Param {
private:
	int value_;
	bool failed_;
public:
	IntegerParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		char* end;
		value_ = strtol(value.c_str(), &end, 0);
		failed_ = *end != 0;
	}
	virtual bool queryInt(int* val) override
	{
		if(failed_){
			return false;
		}
		*val = value_;
		return true;
	}
};

class FloatParam : public Param {
private:
	float value_;
	bool failed_;
public:
	FloatParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		char* end;
		this->value_ = strtof(value.c_str(), &end);
		failed_ = *end != 0;
	}
	virtual ~FloatParam(){}
	virtual bool queryFloat(float* val) override
	{
		if(failed_){
			return false;
		}
		*val = this->value_;
		return true;
	}
};

class BoolParam : public Param {
private:
	bool value_;
	bool failed_;
public:
	BoolParam(const std::string& name, const std::string& value)
	:Param(name)
	{
		std::string copy(value);
		std::transform(copy.begin(), copy.end(), copy.end(), ::tolower);
		if(copy == "true" || copy=="yes"){
			value_ = true;
			failed_ = false;
		}else if(copy == "false" || copy=="no"){
			value_ = false;
			failed_ = false;
		}else{
			failed_ = true;
		}
	}
	virtual ~BoolParam(){}
	virtual bool queryBool(bool* val) override
	{
		if(failed_){
			return false;
		}
		*val = this->value_;
		return true;
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
		throw logging::Exception(__FILE__, __LINE__, "Failed to query float: \"%s\"", name.c_str());
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
		throw logging::Exception(__FILE__, __LINE__, "Failed to query float: \"%s\"", name.c_str());
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
		throw logging::Exception(__FILE__, __LINE__, "Failed to query float: \"%s\"", name.c_str());
	}
	return val;
}

}}
