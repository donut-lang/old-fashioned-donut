/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <sstream>
#include <cinamo/Exception.h>
#include "Disassembler.h"
#include "../VirtualMachine.h"

namespace nes {
using namespace cinamo;

Disassembler::Disassembler(VirtualMachine& vm)
:vm_(vm)
,lastExecuted_(0)
{

}
bool Disassembler::decodeAt(uint16_t addr, Instruction& inst)
{
	inst.pc = addr;
	inst.bin[0] = this->vm_.debuggerRead(addr);
	const uint32_t flags = kInfoTable[inst.bin[0]];
	inst.op_ = static_cast<Operation>(flags & static_cast<uint32_t>(Operation::OperationMask));
	inst.addrMode_ = static_cast<AddrMode>(flags & static_cast<uint32_t>(AddrMode::AddrModeMask));
	inst.clock_ = (flags >> ClockShift) & 0xffff;

	switch(inst.addrMode_){
		case AddrMode::Immediate:
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.addr_ = addr+1;
			break;
		case AddrMode::Zeropage:
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.addr_ = inst.bin[1];
			break;
		case AddrMode::ZeropageX:
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.addr_ = (inst.bin[1] + this->vm_.processor().getX()) & 0xff;
			break;
		case AddrMode::ZeropageY:
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.addr_ = (inst.bin[1] + this->vm_.processor().getY()) & 0xff;
			break;
		case AddrMode::Absolute:
			inst.binLength_ = 3;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.bin[2] = this->vm_.read(addr+2);
			inst.addr_ = inst.bin[1] | (inst.bin[2] << 8);
			break;
		case AddrMode::AbsoluteX:
			inst.binLength_ = 3;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.bin[2] = this->vm_.read(addr+2);
			inst.addr_ = (inst.bin[1] | (inst.bin[2] << 8)) + this->vm_.processor().getX();
			break;
		case AddrMode::AbsoluteY:
			inst.binLength_ = 3;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.bin[2] = this->vm_.read(addr+2);
			inst.addr_ = (inst.bin[1] | (inst.bin[2] << 8)) + this->vm_.processor().getY();
			break;
		case AddrMode::Indirect: {
			inst.binLength_ = 3;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.bin[2] = this->vm_.read(addr+2);
			const uint16_t base = inst.bin[1] | (inst.bin[2] << 8);
			inst.addr_ = this->vm_.read(base) | (this->vm_.read((base & 0xff00) | ((base + 1) & 0x00ff)) << 8); //bug of NES
			break;
		}
		case AddrMode::IndirectX: {
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			const uint16_t base = (this->vm_.read(inst.bin[1]) + this->vm_.processor().getX()) & 0xff;
			inst.addr_ = this->vm_.read(base) | (this->vm_.read((base + 1) & 0xff) << 8);
			break;
		}
		case AddrMode::IndirectY: {
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			const uint16_t base = inst.bin[1];
			inst.addr_ = ((this->vm_.read(base) | (this->vm_.read((base + 1) & 0xff) << 8)) + this->vm_.processor().getY());
			break;
		}
		case AddrMode::Relative:
			inst.binLength_ = 2;
			inst.bin[1] = this->vm_.read(addr+1);
			inst.addr_ = inst.bin[1] >= 128 ? ( addr+2+inst.bin[1] - 256 )  : addr+2+inst.bin[1];
			break;
		case AddrMode::None:
			inst.binLength_=1;
			inst.addr_ = 0;
			break;
		case AddrMode::Invalid:
			break;
		default:
			CINAMO_EXCEPTION(Exception, "[BUG] Oops. Invalid instruction.");
			break;
	}
	return !inst.isInvalidInstruction();
}


std::string Instruction::toString() const noexcept
{
	std::stringstream ss;
	switch(this->op_){
	case Operation::LDA :
		ss << "LDA";
		break;
	case Operation::LDX :
		ss << "LDX";
		break;
	case Operation::LDY :
		ss << "LDY";
		break;
	case Operation::STA :
		ss << "STA";
		break;
	case Operation::STX :
		ss << "STX";
		break;
	case Operation::STY :
		ss << "STY";
		break;
	case Operation::TAX :
		ss << "TAX";
		break;
	case Operation::TAY :
		ss << "TAY";
		break;
	case Operation::TSX :
		ss << "TSX";
		break;
	case Operation::TXA :
		ss << "TXA";
		break;
	case Operation::TXS :
		ss << "TXS";
		break;
	case Operation::TYA :
		ss << "TYA";
		break;
	case Operation::ADC :
		ss << "ADC";
		break;
	case Operation::AND :
		ss << "AND";
		break;
	case Operation::ASL :
		ss << "ASL";
		break;
	case Operation::ASL_:
		ss << "ASL";
		break;
	case Operation::BIT :
		ss << "BIT";
		break;
	case Operation::CMP :
		ss << "CMP";
		break;
	case Operation::CPX :
		ss << "CPX";
		break;
	case Operation::CPY :
		ss << "CPY";
		break;
	case Operation::DEC :
		ss << "DEC";
		break;
	case Operation::DEX :
		ss << "DEX";
		break;
	case Operation::DEY :
		ss << "DEY";
		break;
	case Operation::EOR :
		ss << "EOR";
		break;
	case Operation::INC :
		ss << "INC";
		break;
	case Operation::INX :
		ss << "INX";
		break;
	case Operation::INY :
		ss << "INY";
		break;
	case Operation::LSR :
		ss << "LSR";
		break;
	case Operation::LSR_:
		ss << "LSR";
		break;
	case Operation::ORA :
		ss << "ORA";
		break;
	case Operation::ROL :
		ss << "ROL";
		break;
	case Operation::ROL_:
		ss << "ROL";
		break;
	case Operation::ROR :
		ss << "ROR";
		break;
	case Operation::ROR_:
		ss << "ROR";
		break;
	case Operation::SBC :
		ss << "SBC";
		break;
	case Operation::PHA :
		ss << "PHA";
		break;
	case Operation::PHP :
		ss << "PHP";
		break;
	case Operation::PLA :
		ss << "PLA";
		break;
	case Operation::PLP :
		ss << "PLP";
		break;
	case Operation::CLC :
		ss << "CLC";
		break;
	case Operation::CLD :
		ss << "CLD";
		break;
	case Operation::CLI :
		ss << "CLI";
		break;
	case Operation::CLV :
		ss << "CLV";
		break;
	case Operation::SEC :
		ss << "SEC";
		break;
	case Operation::SED :
		ss << "SED";
		break;
	case Operation::SEI :
		ss << "SEI";
		break;
	case Operation::BRK :
		ss << "BRK";
		break;
	case Operation::NOP :
		ss << "NOP";
		break;
	case Operation::RTS :
		ss << "RTS";
		break;
	case Operation::RTI :
		ss << "RTI";
		break;
	case Operation::JMP :
		ss << "JMP";
		break;
	case Operation::JSR :
		ss << "JSR";
		break;
	case Operation::BCC :
		ss << "BCC";
		break;
	case Operation::BCS :
		ss << "BCS";
		break;
	case Operation::BEQ :
		ss << "BEQ";
		break;
	case Operation::BMI :
		ss << "BMI";
		break;
	case Operation::BNE :
		ss << "BNE";
		break;
	case Operation::BPL :
		ss << "BPL";
		break;
	case Operation::BVC :
		ss << "BVC";
		break;
	case Operation::BVS :
		ss << "BVS";
		break;
	case Operation::Invalid:
		ss << "???";
		break;
	}
	switch(this->addrMode_){
		case AddrMode::Immediate:
			ss << " " << ::cinamo::format("0x%02x",this->bin[1]);
			break;
		case AddrMode::Zeropage:
			ss << " [" << ::cinamo::format("0x%02x",this->bin[1]) << "]";
			break;
		case AddrMode::ZeropageX:
			ss << " [" << ::cinamo::format("0x%02x",this->bin[1]) << ",X]";
			break;
		case AddrMode::ZeropageY:
			ss << " [" << ::cinamo::format("0x%02x",this->bin[1]) << ",Y]";
			break;
		case AddrMode::Absolute:
			ss << " <" << ::cinamo::format("0x%04x",this->bin[1] | bin[2] << 8)  << ">";
			break;
		case AddrMode::AbsoluteX:
			ss << " <" << ::cinamo::format("0x%04x",this->bin[1] | bin[2] << 8)  << ",X>";
			break;
		case AddrMode::AbsoluteY:
			ss << " <" << ::cinamo::format("0x%04x",this->bin[1] | bin[2] << 8)  << ",Y>";
			break;
		case AddrMode::Indirect: {
			ss << " (" << ::cinamo::format("0x%04x",this->bin[1] | bin[2] << 8)  << ")";
			break;
		}
		case AddrMode::IndirectX: {
			ss << " (" << ::cinamo::format("0x%02x",this->bin[1])  << ",X)";
			break;
		}
		case AddrMode::IndirectY: {
			ss << " (" << ::cinamo::format("0x%02x",this->bin[1])  << "),Y";
			break;
		}
		case AddrMode::Relative:
			ss << " " << ((this->bin[1] >=128) ? (static_cast<int>(this->bin[1])-256) : this->bin[1]);
			break;
		case AddrMode::None:
			break;
		case AddrMode::Invalid:
			break;
		default:
			CINAMO_EXCEPTION(Exception, "[BUG] Oops. Invalid instruction.");
			break;
	}	return ss.str();
}

}
