/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <sstream>
#include <cstdlib>
#include <cinamo/Exception.h>
#include <cinamo/String.h>
#include "Assembler.h"

namespace nes {
using namespace cinamo;

const int readOperation(Instruction& inst, std::string const& str)
{
	if(str.size() < 3){
		return -1;
	}
	if(::cinamo::startsWith(str, "LDA")){
		inst.op_=Operation::LDA;
	} else if(::cinamo::startsWith(str, "LDX")){
		inst.op_=Operation::LDX;
	} else if(::cinamo::startsWith(str, "LDY")){
		inst.op_=Operation::LDY;
	} else if(::cinamo::startsWith(str, "STA")){
		inst.op_=Operation::STA;
	} else if(::cinamo::startsWith(str, "STX")){
		inst.op_=Operation::STX;
	} else if(::cinamo::startsWith(str, "STY")){
		inst.op_=Operation::STY;
	} else if(::cinamo::startsWith(str, "TAX")){
		inst.op_=Operation::TAX;
	} else if(::cinamo::startsWith(str, "TAY")){
		inst.op_=Operation::TAY;
	} else if(::cinamo::startsWith(str, "TSX")){
		inst.op_=Operation::TSX;
	} else if(::cinamo::startsWith(str, "TXA")){
		inst.op_=Operation::TXA;
	} else if(::cinamo::startsWith(str, "TXS")){
		inst.op_=Operation::TXS;
	} else if(::cinamo::startsWith(str, "TYA")){
		inst.op_=Operation::TYA;
	} else if(::cinamo::startsWith(str, "ADC")){
		inst.op_=Operation::ADC;
	} else if(::cinamo::startsWith(str, "AND")){
		inst.op_=Operation::AND;
	} else if(::cinamo::startsWith(str, "ASL")){
		inst.op_=Operation::ASL;
	} else if(::cinamo::startsWith(str, "BIT")){
		inst.op_=Operation::BIT;
	} else if(::cinamo::startsWith(str, "CMP")){
		inst.op_=Operation::CMP;
	} else if(::cinamo::startsWith(str, "CPX")){
		inst.op_=Operation::CPX;
	} else if(::cinamo::startsWith(str, "CPY")){
		inst.op_=Operation::CPY;
	} else if(::cinamo::startsWith(str, "DEC")){
		inst.op_=Operation::DEC;
	} else if(::cinamo::startsWith(str, "DEX")){
		inst.op_=Operation::DEX;
	} else if(::cinamo::startsWith(str, "DEY")){
		inst.op_=Operation::DEY;
	} else if(::cinamo::startsWith(str, "EOR")){
		inst.op_=Operation::EOR;
	} else if(::cinamo::startsWith(str, "INC")){
		inst.op_=Operation::INC;
	} else if(::cinamo::startsWith(str, "INX")){
		inst.op_=Operation::INX;
	} else if(::cinamo::startsWith(str, "INY")){
		inst.op_=Operation::INY;
	} else if(::cinamo::startsWith(str, "LSR")){
		inst.op_=Operation::LSR;
	} else if(::cinamo::startsWith(str, "ORA")){
		inst.op_=Operation::ORA;
	} else if(::cinamo::startsWith(str, "ROL")){
		inst.op_=Operation::ROL;
	} else if(::cinamo::startsWith(str, "ROR")){
		inst.op_=Operation::ROR;
	} else if(::cinamo::startsWith(str, "SBC")){
		inst.op_=Operation::SBC;
	} else if(::cinamo::startsWith(str, "PHA")){
		inst.op_=Operation::PHA;
	} else if(::cinamo::startsWith(str, "PHP")){
		inst.op_=Operation::PHP;
	} else if(::cinamo::startsWith(str, "PLA")){
		inst.op_=Operation::PLA;
	} else if(::cinamo::startsWith(str, "PLP")){
		inst.op_=Operation::PLP;
	} else if(::cinamo::startsWith(str, "CLC")){
		inst.op_=Operation::CLC;
	} else if(::cinamo::startsWith(str, "CLD")){
		inst.op_=Operation::CLD;
	} else if(::cinamo::startsWith(str, "CLI")){
		inst.op_=Operation::CLI;
	} else if(::cinamo::startsWith(str, "CLV")){
		inst.op_=Operation::CLV;
	} else if(::cinamo::startsWith(str, "SEC")){
		inst.op_=Operation::SEC;
	} else if(::cinamo::startsWith(str, "SED")){
		inst.op_=Operation::SED;
	} else if(::cinamo::startsWith(str, "SEI")){
		inst.op_=Operation::SEI;
	} else if(::cinamo::startsWith(str, "BRK")){
		inst.op_=Operation::BRK;
	} else if(::cinamo::startsWith(str, "NOP")){
		inst.op_=Operation::NOP;
	} else if(::cinamo::startsWith(str, "RTS")){
		inst.op_=Operation::RTS;
	} else if(::cinamo::startsWith(str, "RTI")){
		inst.op_=Operation::RTI;
	} else if(::cinamo::startsWith(str, "JMP")){
		inst.op_=Operation::JMP;
	} else if(::cinamo::startsWith(str, "JSR")){
		inst.op_=Operation::JSR;
	} else if(::cinamo::startsWith(str, "BCC")){
		inst.op_=Operation::BCC;
	} else if(::cinamo::startsWith(str, "BCS")){
		inst.op_=Operation::BCS;
	} else if(::cinamo::startsWith(str, "BEQ")){
		inst.op_=Operation::BEQ;
	} else if(::cinamo::startsWith(str, "BMI")){
		inst.op_=Operation::BMI;
	} else if(::cinamo::startsWith(str, "BNE")){
		inst.op_=Operation::BNE;
	} else if(::cinamo::startsWith(str, "BPL")){
		inst.op_=Operation::BPL;
	} else if(::cinamo::startsWith(str, "BVC")){
		inst.op_=Operation::BVC;
	} else if(::cinamo::startsWith(str, "BVS")){
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
		if(::cinamo::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::ZeropageX;
			inst.binLength_ = 2;
		}else if(::cinamo::startsWith(left, ",Y")){
			inst.addrMode_ = AddrMode::ZeropageY;
			inst.binLength_ = 2;
		}else if(::cinamo::startsWith(left, "]")){
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
		if(::cinamo::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::AbsoluteX;
			inst.binLength_ = 3;
		}else if(::cinamo::startsWith(left, ",Y")){
			inst.addrMode_ = AddrMode::AbsoluteY;
			inst.binLength_ = 3;
		}else if(::cinamo::startsWith(left, ">")){
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
		if(::cinamo::startsWith(left, ",X")){
			inst.addrMode_ = AddrMode::IndirectX;
			inst.binLength_ = 2;
		}else if(::cinamo::startsWith(left, "),Y")){
			inst.addrMode_ = AddrMode::IndirectY;
			inst.binLength_ = 2;
		}else if(::cinamo::startsWith(left, ")")){
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
