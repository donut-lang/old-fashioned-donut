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
#include "Provider.h"

namespace chisa {
namespace donut {

class IntProvider : public ObjectProvider {
public:
	IntProvider(World* world);
	virtual ~IntProvider() noexcept = default;
public:
	static inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static inline Object* toPointer(int const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Int);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	virtual tinyxml2::XMLElement* serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj ) override;
	virtual Handler<Object> deserialize( tinyxml2::XMLElement* xml ) override;
};

class BoolProvider : public ObjectProvider {
public:
	BoolProvider(World* world);
	virtual ~BoolProvider() noexcept = default;
public:
	static inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static inline Object* toPointer(int const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Int);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	virtual tinyxml2::XMLElement* serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj ) override;
	virtual Handler<Object> deserialize( tinyxml2::XMLElement* xml ) override;
};

class NullProvider : public ObjectProvider {
public:
	NullProvider(World* world);
	virtual ~NullProvider() noexcept = default;
public:
	static inline Object* toPointer() noexcept {
		return reinterpret_cast<Object*>(Object::Tag::Null);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	virtual tinyxml2::XMLElement* serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj ) override;
	virtual Handler<Object> deserialize( tinyxml2::XMLElement* xml ) override;
};


}}
