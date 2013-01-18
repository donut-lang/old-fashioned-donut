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

#include <sstream>
#include <cstdlib>
#include <tarte/Exception.h>
#include <tarte/String.h>
#include "Assembler.h"

namespace nes {
using namespace tarte;

const int readOperation(Instruction& inst, std::string const& str)
{
	if(str.size() < 3){
		return -1;
	}
	if(::tarte::startsWith(str, "LDA")){
		inst.op_=Operation::LDA;
	} else if(::tarte::startsWith(str, "LDX")){
		inst.op_=Operation::LDX;
	} else if(::tarte::startsWith(str, "LDY")){
		inst.op_=Operation::LDY;
	} else if(::tarte::startsWith(str, "STA")){
		inst.op_=Operation::STA;
	} else if(::tarte::startsWith(str, "STX")){
		inst.op_=Operation::STX;
	} else if(::tarte::startsWith(str, "STY")){
		inst.op_=Operation::STY;
	} else if(::tarte::startsWith(str, "TAX")){
		inst.op_=Operation::TAX;
	} else if(::tarte::startsWith(str, "TAY")){
		inst.op_=Operation::TAY;
	} else if(::tarte::startsWith(str, "TSX")){
		inst.op_=Operation::TSX;
	} else if(::tarte::startsWith(str, "TXA")){
		inst.op_=Operation::TXA;
	} else if(::tarte::startsWith(str, "TXS")){
		inst.op_=Operation::TXS;
	} else if(::tarte::startsWith(str, "TYA")){
		inst.op_=Operation::TYA;
	} else if(::tarte::startsWith(str, "ADC")){
		inst.op_=Operation::ADC;
	} else if(::tarte::startsWith(str, "AND")){
		inst.op_=Operation::AND;
	} else if(::tarte::startsWith(str, "ASL")){
		inst.op_=Operation::ASL;
	} else if(::tarte::startsWith(str, "BIT")){
		inst.op_=Operation::BIT;
	} else if(::tarte::startsWith(str, "CMP")){
		inst.op_=Operation::CMP;
	} else if(::tarte::startsWith(str, "CPX")){
		inst.op_=Operation::CPX;
	} else if(::tarte::startsWith(str, "CPY")){
		inst.op_=Operation::CPY;
	} else if(::tarte::startsWith(str, "DEC")){
		inst.op_=Operation::DEC;
	} else if(::tarte::startsWith(str, "DEX")){
		inst.op_=Operation::DEX;
	} else if(::tarte::startsWith(str, "DEY")){
		inst.op_=Operation::DEY;
	} else if(::tarte::startsWith(str, "EOR")){
		inst.op_=Operation::EOR;
	} else if(::tarte::startsWith(str, "INC")){
		inst.op_=Operation::INC;
	} else if(::tarte::startsWith(str, "INX")){
		inst.op_=Operation::INX;
	} else if(::tarte::startsWith(str, "INY")){
		inst.op_=Operation::INY;
	} else if(::tarte::startsWith(str, "LSR")){
		inst.op_=Operation::LSR;
	} else if(::tarte::startsWith(str, "ORA")){
		inst.op_=Operation::ORA;
	} else if(::tarte::startsWith(str, "ROL")){
		inst.op_=Operation::ROL;
	} else if(::tarte::startsWith(str, "ROR")){
		inst.op_=Operation::ROR;
	} else if(::tarte::startsWith(str, "SBC")){
		inst.op_=Operation::SBC;
	} else if(::tarte::startsWith(str, "PHA")){
		inst.op_=Operation::PHA;
	} else if(::tarte::startsWith(str, "PHP")){
		inst.op_=Operation::PHP;
	} else if(::tarte::startsWith(str, "PLA")){
		inst.op_=Operation::PLA;
	} else if(::tarte::startsWith(str, "PLP")){
		inst.op_=Operation::PLP;
	} else if(::tarte::startsWith(str, "CLC")){
		inst.op_=Operation::CLC;
	} else if(::tarte::startsWith(str, "CLD")){
		inst.op_=Operation::CLD;
	} else if(::tarte::startsWith(str, "CLI")){
		inst.op_=Operation::CLI;
	} else if(::tarte::startsWith(str, "CLV")){
		inst.op_=Operation::CLV;
	} else if(::tarte::startsWith(str, "SEC")){
		inst.op_=Operation::SEC;
	} else if(::tarte::startsWith(str, "SED")){
		inst.op_=Operation::SED;
	} else if(::tarte::startsWith(str, "SEI")){
		inst.op_=Operation::SEI;
	} else if(::tarte::startsWith(str, "BRK")){
		inst.op_=Operation::BRK;
	} else if(::tarte::startsWith(str, "NOP")){
		inst.op_=Operation::NOP;
	} else if(::tarte::startsWith(str, "RTS")){
		inst.op_=Operation::RTS;
	} else if(::tarte::startsWith(str, "RTI")){
		inst.op_=Operation::RTI;
	} else if(::tarte::startsWith(str, "JMP")){
		inst.op_=Operation::JMP;
	} else if(::tarte::startsWith(str, "JSR")){
		inst.op_=Operation::JSR;
	} else if(::tarte::startsWith(str, "BCC")){
		inst.op_=Operation::BCC;
	} else if(::tarte::startsWith(str, "BCS")){
		inst.op_=Operation::BCS;
	} else if(::tarte::startsWith(str, "BEQ")){
		inst.op_=Operation::BEQ;
	} else if(::tarte::startsWith(str, "BMI")){
		inst.op_=Operation::BMI;
	} else if(::tarte::startsWith(str, "BNE")){
		inst.op_=Operation::BNE;
	} else if(::tarte::startsWith(str, "BPL")){
		inst.op_=Operation::BPL;
	} else if(::tarte::startsWith(str, "BVC")){
		inst.op_=Operation::BVC;
	} else if(::tarte::startsWith(str, "BVS")){
		inst.op_=Operation::BVS;
	}
	return 3;
}

int readInt(Instruction& inst, std::string const& str, std::string& left_){
	char* left;
	int i = std::strtol(str.c_str(), &left,0);
	left_ = left;
	return i;
}

static int readAddrMode(Instruction& inst, std::string const& str){
	switch(str[0]){
	case '[':
	{
		std::string left;
		inst.bin[1] = readInt(inst, &str.c_str()[1], left);
		if(::tarte::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::ZeropageX;
			inst.binLength_ = 2;
		}else if(::tarte::startsWith(left, ",Y")){
			inst.addrMode_ = AddrMode::ZeropageY;
			inst.binLength_ = 2;
		}else if(::tarte::startsWith(left, "]")){
			inst.addrMode_ = AddrMode::Zeropage;
			inst.binLength_ = 2;
		}else{
			inst.op_ = Operation::Invalid;
			return -1;
		}
		break;
	}
	case '<':
	{
		std::string left;
		unsigned int i = readInt(inst, &str.c_str()[1], left);
		inst.bin[1] = i & 0xff;
		inst.bin[2] = (i >> 8) & 0xff;
		if(::tarte::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::AbsoluteX;
			inst.binLength_ = 3;
		}else if(::tarte::startsWith(left, ",Y")){
			inst.addrMode_ = AddrMode::AbsoluteY;
			inst.binLength_ = 3;
		}else if(::tarte::startsWith(left, ">")){
			inst.addrMode_ = AddrMode::Absolute;
			inst.binLength_ = 3;
		}else{
			inst.op_ = Operation::Invalid;
			return -1;
		}
		break;
	}
	case '(':
	{
		std::string left;
		unsigned int i = readInt(inst, &str.c_str()[1], left);
		inst.bin[1] = i & 0xff;
		inst.bin[2] = (i >> 8) & 0xff;
		if(::tarte::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::IndirectX;
			inst.binLength_ = 2;
		}else if(::tarte::startsWith(left, "),Y")){
			inst.addrMode_ = AddrMode::IndirectY;
			inst.binLength_ = 2;
		}else if(::tarte::startsWith(left, ")")){
			inst.addrMode_ = AddrMode::Indirect;
			inst.binLength_ = 3;
		}else{
			inst.op_ = Operation::Invalid;
			return -1;
		}
	}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
	case '+':
	{
		std::string left;
		int i = readInt(inst, str.c_str(), left);
		inst.bin[1] = static_cast<unsigned int>(i) & 0xff;
		inst.addrMode_ = AddrMode::Immediate;
		inst.binLength_ = 2;
		break;
	}
	default:
		inst.addrMode_ = AddrMode::None;
		inst.binLength_ = 1;
		break;
	}
	return 0;
}

const static unsigned int symCode[256] = {static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::BRK),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::ORA),255,255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::ORA),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::ASL),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::PHP),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::ORA),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::ASL),255,255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::ORA),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::ASL),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BPL),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::ORA),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::ORA),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::ASL),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::CLC),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::ORA),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::ORA),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::ASL),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::JSR),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::AND),255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::BIT),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::AND),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::ROL),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::PLP),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::AND),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::ROL),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::BIT),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::AND),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::ROL),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BMI),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::AND),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::AND),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::ROL),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::SEC),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::AND),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::AND),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::ROL),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::RTI),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::EOR),255,255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::EOR),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::LSR),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::PHA),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::EOR),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::LSR),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::JMP),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::EOR),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::LSR),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BVC),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::EOR),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::EOR),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::LSR),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::CLI),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::EOR),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::EOR),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::LSR),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::RTS),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::ADC),255,255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::ADC),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::ROR),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::PLA),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::ADC),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::ROR),255,static_cast<unsigned int>(AddrMode::Indirect) | static_cast<unsigned int>(Operation::JMP),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::ADC),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::ROR),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BVS),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::ADC),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::ADC),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::ROR),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::SEI),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::ADC),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::ADC),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::ROR),255,255,static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::STA),255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::STY),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::STA),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::STX),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::DEY),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TXA),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::STY),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::STA),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::STX),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BCC),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::STA),255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::STY),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::STA),static_cast<unsigned int>(AddrMode::ZeropageY) | static_cast<unsigned int>(Operation::STX),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TYA),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::STA),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TXS),255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::STA),255,255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::LDY),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::LDX),255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::LDY),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::LDX),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TAY),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TAX),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::LDY),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::LDX),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BCS),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::LDA),255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::LDY),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::ZeropageY) | static_cast<unsigned int>(Operation::LDX),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::CLV),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::TSX),255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::LDY),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::LDA),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::LDX),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::CPY),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::CMP),255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::CPY),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::CMP),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::DEC),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::INY),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::CMP),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::DEX),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::CPY),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::CMP),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::DEC),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BNE),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::CMP),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::CMP),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::DEC),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::CLD),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::CMP),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::CMP),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::DEC),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::CPX),static_cast<unsigned int>(AddrMode::IndirectX) | static_cast<unsigned int>(Operation::SBC),255,255,static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::CPX),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::SBC),static_cast<unsigned int>(AddrMode::Zeropage) | static_cast<unsigned int>(Operation::INC),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::INX),static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::SBC),static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::NOP),255,static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::CPX),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::SBC),static_cast<unsigned int>(AddrMode::Absolute) | static_cast<unsigned int>(Operation::INC),255,static_cast<unsigned int>(AddrMode::Immediate) | static_cast<unsigned int>(Operation::BEQ),static_cast<unsigned int>(AddrMode::IndirectY) | static_cast<unsigned int>(Operation::SBC),255,255,255,static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::SBC),static_cast<unsigned int>(AddrMode::ZeropageX) | static_cast<unsigned int>(Operation::INC),255,static_cast<unsigned int>(AddrMode::None) | static_cast<unsigned int>(Operation::SED),static_cast<unsigned int>(AddrMode::AbsoluteY) | static_cast<unsigned int>(Operation::SBC),255,255,255,static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::SBC),static_cast<unsigned int>(AddrMode::AbsoluteX) | static_cast<unsigned int>(Operation::INC),255,
};

Instruction encodeInst(std::string const& dis_)
{
	Instruction inst;
	std::string dis;
	bool not_num = true;
	for(char s : dis_){
		if(!not_num){
			dis += s;
		}else if(::isalpha(s)){
			dis += static_cast<char>(::toupper(s));
		}else if(::isdigit(s)){
			not_num = false;
			dis += s;
		}else if(!::isspace(s)){
			dis += s;
		}
	}
	if(readOperation(inst, dis) < 0) {
		inst.op_ = Operation::Invalid;
	}else if(readAddrMode(inst, dis.substr(3)) < 0){
		inst.op_ = Operation::Invalid;
	}
	if(inst.op_ != Operation::Invalid){
		unsigned int code = static_cast<unsigned int>(inst.op_) | static_cast<unsigned int>(inst.addrMode_);
		for(int i=0;i<256;++i){
			if( symCode[i] == code ) {
				inst.bin[0] = i;
				return inst;
			}
		}
	}
	inst.op_ = Operation::Invalid;
	return inst;
}

}
