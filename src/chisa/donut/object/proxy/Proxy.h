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
#include "../../../util/ClassUtil.h"
#include "../Object.h"
#include "../../native/PureNativeClosure.h"

namespace chisa {
namespace donut {
class World;

class Proxy {
private:
	World* const world_;
	std::map<std::string, Handler<PureNativeClosure> > closureMap_;
protected:
	Proxy(World* const world):world_(world){};
	~Proxy() noexcept = default;
	World* world() const { return this->world_; };
	template <typename... Args>
	void registerClosure(const std::string& name, std::function<Object*(Args... args)> f);
public:
	bool have(const Object* ptr, const std::string& name) const;
	bool haveOwn(const Object* ptr, const std::string& name) const;
	Handler<Object> load(const Object* ptr, const std::string& name);
};

class BoolProxy : public Proxy {
public:
	BoolProxy(World* const world_);
	~BoolProxy() noexcept = default;
public:
	static inline bool fromPointer(const Object* const ptr) noexcept {
		return (reinterpret_cast<std::uintptr_t>(ptr) & ~Object::Tag::Bool) != 0;
	}
	static inline Object* toPointer(bool const& val) noexcept {
		return val?
				reinterpret_cast<Object*>((1 << 2) | Object::Tag::Bool):
				reinterpret_cast<Object*>((0 << 2) | Object::Tag::Bool);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
};

class NullProxy : public Proxy {
public:
	NullProxy(World* const world_);
	~NullProxy() noexcept = default;
public:
	static inline Object* toPointer() noexcept {
		return reinterpret_cast<Object*>(Object::Tag::Null);
	}
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
};

class IntProxy : public Proxy {
public:
	IntProxy(World* const world);
	~IntProxy() noexcept = default;
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
	Handler<Object> store(const Object* ptr, const std::string& name, Handler<Object> obj);
};

}}
