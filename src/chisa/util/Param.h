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
#include <string>
#include <memory>
#include "ClassUtil.h"

namespace chisa {
namespace util {


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
