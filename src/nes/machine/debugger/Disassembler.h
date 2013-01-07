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
#include <cstddef>
#include <cstdint>

namespace nes {

enum class AddrMode : uint32_t {
	AddrModeMask = 0xf,
	Immediate = 0,
	Zeropage = 1,
	ZeropageX = 2,
	ZeropageY = 3,
	Absolute = 4,
	AbsoluteX = 5,
	AbsoluteY = 6,
	Indirect = 7,
	IndirectX = 8,
	IndirectY = 9,
	Relative = 10,
	None = 11
};

enum class Operation : uint32_t {
	OperationMask=0xfff0,
	LDA = 0,
	LDX = 16,
	LDY = 32,
	STA = 48,
	STX = 64,
	STY = 80,
	TAX = 96,
	TAY = 112,
	TSX = 128,
	TXA = 144,
	TXS = 160,
	TYA = 176,
	ADC = 192,
	AND = 208,
	ASL = 224,
	ASL_ = 240,
	BIT = 256,
	CMP = 272,
	CPX = 288,
	CPY = 304,
	DEC = 320,
	DEX = 336,
	DEY = 352,
	EOR = 368,
	INC = 384,
	INX = 400,
	INY = 416,
	LSR = 432,
	LSR_ = 448,
	ORA = 464,
	ROL = 480,
	ROL_ = 496,
	ROR = 512,
	ROR_ = 528,
	SBC = 544,
	PHA = 560,
	PHP = 576,
	PLA = 592,
	PLP = 608,
	CLC = 624,
	CLD = 640,
	CLI = 656,
	CLV = 672,
	SEC = 688,
	SED = 704,
	SEI = 720,
	BRK = 736,
	NOP = 752,
	RTS = 768,
	RTI = 784,
	JMP = 800,
	JSR = 816,
	BCC = 832,
	BCS = 848,
	BEQ = 864,
	BMI = 880,
	BNE = 896,
	BPL = 912,
	BVC = 928,
	BVS = 944,
};

const int constexpr ClockShift = 16;

struct Instruction {
	enum Operation op_;
	enum AddrMode addrMode_;
	unsigned int clock_;
//命令
	unsigned int binLength_;
	unsigned char bin[3];
//解決したアドレスとその値
	uint16_t addr_;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & op_;
		arc & addrMode_;
		arc & clock_;
		arc & binLength_;
		arc & bin;
		arc & addr_;
	}
};

class VirtualMachine;

class Disassembler final {
private:
	static const uint32_t kInfoTable[0x100];
	VirtualMachine& vm_;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
	}
public:
	Disassembler(VirtualMachine& vm);
	~Disassembler() = default;
public:
	void decodeAt(uint16_t addr, Instruction* inst);
};

}
