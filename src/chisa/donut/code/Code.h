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

namespace chisa {
namespace donut {

typedef unsigned int Instruction;
enum Inst {
	Nop			= 0<<24,
	Push			= 1<<24,
	PushCopy		= 2<<24,
	Pop			= 3<<24,
	SearchScope	= 4<<24,
	StoreObj		= 5<<24,
	LoadObj		= 6<<24,
	LoadLocal	= 7<<24,
	StoreLocal	= 8<<24,
	Apply		= 9<<24,
	ConstInt		= 1<<16,
	ConstBool	= 2<<16,
	ConstFloat	= 3<<16,
	ConstClosure	= 3<<16,
	ConstString	= 4<<16
};

class Code : public HandlerBody<Code> {

public:
	Code();
	virtual ~Code() noexcept = default;
	void onFree() noexcept { delete this; }
	template <typename T> unsigned int constCode(T const& val);
};

}}
