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
private:
	uint8_t inline memoryRead(uint16_t const addr, uint8_t const value) noexcept {
		return value;
	}
	uint8_t inline memoryWrite(uint16_t const addr, uint8_t now) noexcept {
		return now;
	}
	void inline memoryExecute(uint16_t const addr) noexcept {

	}
public:
	void inline processorExecute(uint16_t const addr) noexcept {
		this->memoryExecute(addr);
	}
public: /* ram */
	uint8_t inline ramRead(uint16_t const addr, uint8_t const value) noexcept {
		return memoryRead(addr, value);
	}
	uint8_t inline ramWrite(uint16_t const addr, uint8_t now) noexcept {
		return memoryWrite(addr, now);
	}
public: /* video */
	uint8_t inline videoReadReg(uint16_t const addr, uint8_t const value) noexcept {
		return memoryRead(addr, value);
	}
	uint8_t inline videoWriteReg(uint16_t const addr, uint8_t now) noexcept {
		return memoryWrite(addr, now);
	}
	uint8_t inline videoWriteDMA(uint8_t now) noexcept {
		return memoryWrite(0x4014, now);
	}
public: /* audio */
	uint8_t inline audioRead(uint16_t const addr, uint8_t const value) noexcept {
		return memoryRead(addr, value);
	}
	uint8_t inline audioWrite(uint16_t const addr, uint8_t now) noexcept {
		return memoryWrite(addr, now);
	}
public: /* io */
	uint8_t inline ioRead(uint16_t const addr, uint8_t const value) noexcept {
		return memoryRead(addr, value);
	}
	uint8_t inline ioWrite(uint8_t now) noexcept {
		return memoryWrite(0x4016, now);
	}
private: /* cartridge */
	uint8_t inline cartridgeReadCPU(uint16_t const addr, uint8_t const value) {
		return memoryRead(addr, value);
	}
	uint8_t inline cartridgeWriteCPU(uint16_t const addr, uint8_t const now) {
		return memoryWrite(addr, now);
	}
public:
	uint8_t inline cartridgeReadCPUReg(uint16_t const addr, uint8_t const value) {
		return cartridgeReadCPU(addr, value);
	}
	uint8_t inline cartridgeWriteCPUReg(uint16_t const addr, uint8_t const now) {
		return cartridgeWriteCPU(addr, now);
	}
	uint8_t inline cartridgeReadCPUSaveArea(uint16_t const addr, uint8_t const value) {
		return cartridgeReadCPU(addr, value);
	}
	uint8_t inline cartridgeWriteCPUSaveArea(uint16_t const addr, uint8_t const now) {
		return cartridgeWriteCPU(addr, now);
	}
	uint8_t inline cartridgeReadCPUBankLow(uint16_t const addr, uint8_t const value) {
		return cartridgeReadCPU(addr, value);
	}
	uint8_t inline cartridgeWriteCPUBankLow(uint16_t const addr, uint8_t const now) {
		return cartridgeWriteCPU(addr, now);
	}
	uint8_t inline cartridgeReadCPUBankHigh(uint16_t const addr, uint8_t const value) {
		return cartridgeReadCPU(addr, value);
	}
	uint8_t inline cartridgeWriteCPUBankHigh(uint16_t const addr, uint8_t const now) {
		return cartridgeWriteCPU(addr, now);
	}

};

}
