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
#include <vector>
#include <tarte/Handler.h>
#include <tarte/XVal.h>
#include "Inst.h"

namespace donut {
using namespace tarte;

class Closure final : public HandlerBody<Closure>{
	const std::vector<std::string> arglist_;
	const std::vector<donut::Instruction> instlist_;
private:
	Closure(Closure const& other) = delete;
	Closure(Closure&& other) = delete;
	Closure& operator=(Closure const& other) = delete;
	Closure& operator=(Closure&& other) = delete;
public:
	Closure(std::vector<std::string> const& arglist, std::vector<donut::Instruction> const& instlist);
	Closure(XValue const& data);
	XValue save();
	virtual ~Closure() noexcept = default;
public:
	bool onFree() noexcept { return false; };
public:
	inline std::vector<std::string> const& arglist() const noexcept { return this->arglist_; };
	inline std::vector<donut::Instruction> const& instlist() const noexcept { return this->instlist_; };
public:
	bool operator==( Closure const& other ) const noexcept{
		return other.arglist_ == this->arglist_ && this->instlist_ == other.instlist_;
	}
	bool operator!=( Closure const& other ) const noexcept{
		return !this->operator ==(other);
	}
};

}
