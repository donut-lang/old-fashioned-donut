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
#include "../../Handler.h"
#include <vector>
#include "Inst.h"

namespace chisa {
namespace donut {

class Closure : public HandlerBody<Closure>{
	unsigned int code_;
	const std::vector<std::string> arglist_;
	const std::vector<donut::Instruction> instlist_;
public:
	Closure(const std::vector<std::string>& arglist, const std::vector<donut::Instruction>& instlist);
	virtual ~Closure() noexcept = default;
	void code(unsigned int code);
public:
	bool onFree() noexcept { return false; };
public:
	unsigned int code() const noexcept { return this->code_; };
	std::vector<std::string> const& arglist() const noexcept { return this->arglist_; };
	std::vector<donut::Instruction> const& instlist() const noexcept { return this->instlist_; };
};

}}
