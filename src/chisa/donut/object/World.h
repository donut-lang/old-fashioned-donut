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
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../Handler.h"
#include "../code/Code.h"
#include "Object.h"

namespace chisa {
namespace donut {
class World;

class Proxy {
private:
	World* const world_;
protected:
	Proxy(World* const world):world_(world){};
	~Proxy() noexcept = default;
	World* world() { return this->world_; };
};

class BoolProxy : public Proxy {
public:
	BoolProxy(World* const world_);
	~BoolProxy() noexcept = default;
public:
	static inline bool fromPointer(const Object* const ptr) noexcept {
		return (reinterpret_cast<std::uintptr_t>(ptr) & ~Object::Tag::Bool) != 0;
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	bool have(const Object* ptr, const std::string& name) const;
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
	Handler<Object> load(const Object* ptr, const std::string& name);
};

class NullProxy : public Proxy {
public:
	NullProxy(World* const world_);
	~NullProxy() noexcept = default;
public:
	static inline bool fromPointer(const Object* const ptr) noexcept {
		return (reinterpret_cast<std::uintptr_t>(ptr) & ~Object::Tag::Bool) != 0;
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	bool have(const Object* ptr, const std::string& name) const;
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
	Handler<Object> load(const Object* ptr, const std::string& name);
};

class IntProxy : public Proxy {
public:
	IntProxy(World* const world);
	~IntProxy() noexcept = default;
public:
	static inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	bool have(const Object* ptr, const std::string& name) const;
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
	Handler<Object> load(const Object* ptr, const std::string& name);
};

//-----------------------------------------------------------------------------

class World {
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Code> code_;
	unsigned int generation_;
	BoolProxy boolProxy_;
	IntProxy intProxy_;
	NullProxy nullProxy_;
public:
	World(logging::Logger& log, Handler<Code> code);
	virtual ~World() noexcept = default;
public:
	unsigned int nextGeneration();
	BoolProxy& boolProxy() {return boolProxy_;};
	IntProxy& intProxy() {return intProxy_;};
	NullProxy& nullProxy() {return nullProxy_;};
	Handler<Code> code() { return this->code_; }
public:
	template <typename T, typename... Args>
	Handler<T> create(const Args&... args)
	{
		Handler<T> obj(new T(*this, args...));
		return obj;
	}
};

}}
