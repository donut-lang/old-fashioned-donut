/**
 * Chisa
 * Copyright (C) 2013 psi
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
#include "Disassembler.h"
#include "Watcher.h"

namespace nes {

class Debugger final {
private:
	VirtualMachine& vm_;
	Disassembler disasm_;
	Watcher watcher_;
public:
	Debugger(VirtualMachine& vm);
	~Debugger() noexcept = default;
public:
	uint8_t inline memoryRead(uint16_t const addr, uint8_t const value) noexcept {

		return value;
	}
	void inline memoryWrite(uint16_t const addr, uint8_t const old, uint8_t now) noexcept
	{

	}
	void memoryExecute(uint16_t const addr) noexcept;
};

}
