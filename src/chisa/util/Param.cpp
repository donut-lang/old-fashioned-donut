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
#include <algorithm>
#include <cstdlib>

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
	bool queryString(std::string* val) override
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
	bool queryInt(int* val) override
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
	bool queryFloat(float* val) override
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
	std::string _type(type);
	std::transform(_type.begin(), _type.end(), _type.begin(), ::tolower);
	if(_type==Param::TypeString::String){
		return std::shared_ptr<Param>(new StringParam(name, value));
	}else if(_type==Param::TypeString::Integer){
		return std::shared_ptr<Param>(new IntegerParam(name, value));
	}else if(_type==Param::TypeString::Float){
		return std::shared_ptr<Param>(new FloatParam(name, value));
	}else{
		return std::shared_ptr<Param>(new StringParam(name, value));
	}
}

void ParamSet::add(std::shared_ptr<Param> p)
{
	this->params_.insert(std::pair<std::string, std::shared_ptr<Param> >(p->name(), p));
}
void ParamSet::add(const std::string& name, const std::string& type, const std::string& value)
{
	std::shared_ptr<Param> p = Param::createParam(name, type, value);
	this->params_.insert(std::pair<std::string, std::shared_ptr<Param> >(name, p));
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
	return this->params_.find(name) == this->params_.end();
}

std::shared_ptr<Param> Param::parseTree(tinyxml2::XMLElement* elem)
{
	std::string elemName(elem->Name());
	std::transform(elemName.begin(), elemName.end(), elemName.begin(), ::tolower);

	const char* name;
	const char* type;
	const char* value;
	if(elemName != "param" || !(name = elem->Attribute("name", nullptr) ) || ! (value = elem->Attribute("value", nullptr)) ){
		return std::shared_ptr<Param>();
	}
	if( !(type = elem->Attribute("type", nullptr) ) ){
		type="string";
	}
	elem->FirstChildElement();
	return Param::createParam(name, type, value);
}
void ParamSet::parseTree(std::shared_ptr<ParamSet> paramSet, tinyxml2::XMLElement* elem)
{
	for(XMLNode* it=elem->FirstChild(); it; it=it->NextSibling()){
		if(XMLElement* paramElem = it->ToElement()){
			std::shared_ptr<Param> p = Param::parseTree(paramElem);
			if(p){
				paramSet->add(p);
			}
		}
	}
}


}}
