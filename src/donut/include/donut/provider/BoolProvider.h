/**
 * Donut
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
#include "../object/Object.h"
#include "Provider.h"
#include <cstdint>

namespace donut {

class BoolProvider final : public Provider {
public:
	BoolProvider(Handler<Heap> const& heap);
	virtual ~BoolProvider() noexcept = default;
public:
	static constexpr inline bool fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(bool const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Bool);
	}
public:
	std::string repr(const Object* ptr) const;
	std::string print(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
public:
	Handler<Object> create( bool const& val );
};

}
