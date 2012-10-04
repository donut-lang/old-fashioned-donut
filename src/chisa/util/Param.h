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

#ifndef CHISA_UTIL_PARAM_H_
#define CHISA_UTIL_PARAM_H_

#include <tinyxml2.h>
#include <map>
#include <string>
#include <memory>
#include "class_utils.h"

namespace chisa {
namespace util {


class Param {
	DISABLE_COPY_AND_ASSIGN(Param);
public:
	class TypeString {
	public:
		static constexpr const char* Integer = "int";
		static constexpr const char* String = "string";
		static constexpr const char* Float = "float";
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
public:
	static std::shared_ptr<Param> createParam(const std::string& name, const std::string& type, const std::string& value);
public:
	static std::shared_ptr<Param> parseTree(tinyxml2::XMLElement* elem);
};

class ParamSet {
	std::map<std::string, std::shared_ptr<Param> > params_;
public:
	ParamSet(){};
	virtual ~ParamSet(){};
public:
	void add(const std::string& name, const std::string& type, const std::string& value);
	void add(std::shared_ptr<Param> p);
	std::shared_ptr<Param> get(const std::string& name);
	bool has(const std::string& name);
	std::size_t size() const{ return this->params_.size(); };
public:
	static void parseTree(std::shared_ptr<ParamSet> paramSet, tinyxml2::XMLElement* elem);
};

}}
#endif
