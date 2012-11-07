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
#include <cstdint>
#include <map>
#include <vector>
#include <functional>
#include "../../Handler.h"

namespace chisa {
namespace donut {
class ObjectPool;

class Object : public HandlerBody<Object> {
private:
	ObjectPool& pool_;
	std::map<std::string, std::map<unsigned int, Handler<Object> > > slots_;
public:
	enum Tag{
		Mask=3U,
		Obj=0U,
		Int=1U,
		Bool=2U
	};
public:
	Object(ObjectPool& pool);
	virtual ~Object() noexcept = default;
public:
	std::string toString() const;
	int toInt() const;
	float toFloat() const;
	bool toBool() const;
public:
	inline bool isObject() const noexcept { return Tag::Obj==tag(); };
	inline intptr_t tag() const noexcept { return reinterpret_cast<std::uintptr_t>(this) & Tag::Mask; };
	inline void onFree() {};//参照カウント、どうしましょう？？
	inline void incref() { if(isObject()) { this->HandlerBody<Object>::incref(); } };
	inline void decref() { if(isObject()) { this->HandlerBody<Object>::decref(); } };
};

class NativeClosure {
	std::function<void(Handler<Object> self, Handler<Object> arg)> func;
};

}}
