/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace nes {

enum class AddrMode : uint16_t {
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
	None = 11,
	Invalid=AddrModeMask
};

enum class Operation : uint16_t {
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
	Invalid=OperationMask
};

const int constexpr ClockShift = 16;

struct Instruction {
	unsigned short pc;
	enum Operation op_;
	enum AddrMode addrMode_;
	unsigned char clock_;
//命令
	unsigned char binLength_;
	unsigned char bin[3];
//解決したアドレスとその値
	uint16_t addr_;
public:
	inline bool isInvalidInstruction() const noexcept{
		return (op_ == Operation::Invalid) || (addrMode_ == AddrMode::Invalid);
	}
	std::string toString() const noexcept;
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
	uint16_t lastExecuted_;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & lastExecuted_;
	}
public:
	Disassembler(VirtualMachine& vm);
	~Disassembler() = default;
public:
	/**
	 * 命令として成立するならtrueを、無効な命令ならfalseを返す
	 */
	bool decodeAt(uint16_t addr, Instruction& inst);
public:
	void inline onMemoryExecute(uint16_t const addr){
		lastExecuted_ = addr;
	}
	inline uint16_t nowPC() const noexcept{
		return lastExecuted_;
	}
};

}