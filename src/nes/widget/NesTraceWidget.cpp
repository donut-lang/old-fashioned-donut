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

#include "NesTraceWidget.h"
#include "../NesGeist.h"
#include "../../chisa/tk/World.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"
#include "../../chisa/tk/element/WidgetElement.h"

namespace nes {

NesTraceWidget::NesTraceWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,asmRenderer_(log, _world.lock()->drawableManager(), 24.0f)
,numRenderer_(log, _world.lock()->drawableManager(), 24.0f)
,symRenderer_(log, _world.lock()->drawableManager(), 24.0f)
,addrWidth_(0)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;

	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::LDA ),"LDA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::LDX ),"LDX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::LDY ),"LDY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::STA ),"STA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::STX ),"STX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::STY ),"STY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TAX ),"TAX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TAY ),"TAY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TSX ),"TSX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TXA ),"TXA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TXS ),"TXS");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::TYA ),"TYA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ADC ),"ADC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::AND ),"AND");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ASL ),"ASL");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ASL_),"ASL");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BIT ),"BIT");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CMP ),"CMP");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CPX ),"CPX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CPY ),"CPY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::DEC ),"DEC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::DEX ),"DEX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::DEY ),"DEY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::EOR ),"EOR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::INC ),"INC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::INX ),"INX");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::INY ),"INY");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::LSR ),"LSR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::LSR_),"LSR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ORA ),"ORA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ROL ),"ROL");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ROL_),"ROL");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ROR ),"ROR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::ROR_),"ROR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::SBC ),"SBC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::PHA ),"PHA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::PHP ),"PHP");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::PLA ),"PLA");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::PLP ),"PLP");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CLC ),"CLC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CLD ),"CLD");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CLI ),"CLI");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::CLV ),"CLV");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::SEC ),"SEC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::SED ),"SED");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::SEI ),"SEI");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BRK ),"BRK");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::NOP ),"NOP");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::RTS ),"RTS");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::RTI ),"RTI");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::JMP ),"JMP");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::JSR ),"JSR");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BCC ),"BCC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BCS ),"BCS");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BEQ ),"BEQ");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BMI ),"BMI");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BNE ),"BNE");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BPL ),"BPL");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BVC ),"BVC");
	this->asmRenderer_.registerSymbol(static_cast<unsigned int>(Operation::BVS ),"BVS");

	this->numRenderer_.registerSymbol(0, "0");
	this->numRenderer_.registerSymbol(1, "1");
	this->numRenderer_.registerSymbol(2, "2");
	this->numRenderer_.registerSymbol(3, "3");
	this->numRenderer_.registerSymbol(4, "4");
	this->numRenderer_.registerSymbol(5, "5");
	this->numRenderer_.registerSymbol(6, "6");
	this->numRenderer_.registerSymbol(7, "7");
	this->numRenderer_.registerSymbol(8, "8");
	this->numRenderer_.registerSymbol(9, "9");
	this->numRenderer_.registerSymbol(10, "a");
	this->numRenderer_.registerSymbol(11, "b");
	this->numRenderer_.registerSymbol(12, "c");
	this->numRenderer_.registerSymbol(13, "d");
	this->numRenderer_.registerSymbol(14, "e");
	this->numRenderer_.registerSymbol(15, "f");

	this->symRenderer_.registerSymbol(Dollar        ,"$" );
	this->symRenderer_.registerSymbol(RightParen    ,"(" );
	this->symRenderer_.registerSymbol(LeftParen     ,")" );
	this->symRenderer_.registerSymbol(Eq            ,"=" );
	this->symRenderer_.registerSymbol(Comma         ,"," );
	this->symRenderer_.registerSymbol(AtMark        ,"@" );
	this->symRenderer_.registerSymbol(SymX          ,"X" );
	this->symRenderer_.registerSymbol(SymY          ,"Y" );
}

void NesTraceWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
	using namespace chisa::geom;
	using namespace chisa::gl;
	Handler<NesGeist> geist = this->geist_.lock();
	Handler<chisa::tk::WidgetElement> parent = this->wrapper().lock();
	if(!geist || !parent){
		return;
	}
	VirtualMachine& vm = *geist->machine();
	Debugger& dbg = geist->machine()->debugger();
	Disassembler& disasm = dbg.disassembler();
	uint16_t const nowPC = disasm.nowPC();


	PredefinedSymRenderer::SymList addrSym(4);
	PredefinedSymRenderer::SymList numSym(2);
	float offset = 0;
	float rowHeight = this->symRenderer_.maxHeight();
	float spaceSize = this->numRenderer_.maxWidth();
	unsigned int const startRow = max(std::floor((area.y() - rowHeight)/rowHeight), 0);
	unsigned int const endRow = std::ceil((area.y() - rowHeight + area.height())/rowHeight);

	Instruction inst;
	uint16_t pc = nowPC;
	for(unsigned int i=0;i<=endRow;++i){
		disasm.decodeAt(pc, inst);
		pc+=inst.binLength_;
		if(i < startRow){
			continue;
		}
		float startX = 0;
		{ //addr
			startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
			addrSym[0] = (pc >> 12) & 0xf;
			addrSym[1] = (pc >>  8) & 0xf;
			addrSym[2] = (pc >>  4) & 0xf;
			addrSym[3] = (pc >>  0) & 0xf;
			startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
		}
		startX += spaceSize;
		{ //binary
			float internalStartX = 0;
			for(decltype(inst.binLength_) i=0;i<inst.binLength_;++i){
				numSym[0] = (inst.bin[i] >> 4) & 0xf;
				numSym[1] = (inst.bin[i] >> 0) & 0xf;
				Area numRendererd = this->numRenderer_.renderSyms(cv, Point(startX+internalStartX,offset), numSym, 0.0f);
				internalStartX += numRendererd.width();
				internalStartX += spaceSize;
			}
		}
		startX += spaceSize * 9;
		startX += spaceSize;
		{ //operand
			Area opRendererd = this->asmRenderer_.renderSym(cv, Point(startX,offset), static_cast<PredefinedSymRenderer::Symbol>(inst.op_), 0.0f);
			startX += opRendererd.width();
		}
		{ //addr
			switch(inst.addrMode_) {
			case AddrMode::Immediate: { //$xx
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
			}
				break;
			case AddrMode::Zeropage: { //$xx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint8_t operand = vm.debuggerRead( pc+1 );
				numSym[0] = (operand >> 4) & 0xf;
				numSym[1] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::ZeropageX: { //$xx,X @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint8_t operand = vm.debuggerRead( pc+1 );
				numSym[0] = (operand >> 4) & 0xf;
				numSym[1] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymX, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;

				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::ZeropageY: { //$xx,Y @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint8_t operand = vm.debuggerRead( pc+1 );
				numSym[0] = (operand >> 4) & 0xf;
				numSym[1] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymY, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;

				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::Absolute : { //$xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >>12) & 0xf;
				addrSym[1] = (inst.addr_ >> 8) & 0xf;
				addrSym[2] = (inst.addr_ >> 4) & 0xf;
				addrSym[3] = (inst.addr_ >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::AbsoluteX: { //$xxxx,X @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint16_t operand = vm.debuggerRead( pc+1 ) | (vm.debuggerRead( pc+2 ) << 8) ;
				addrSym[0] = (operand >>12) & 0xf;
				addrSym[1] = (operand >> 8) & 0xf;
				addrSym[2] = (operand >> 4) & 0xf;
				addrSym[3] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymX, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;

				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::AbsoluteY: { //$xxxx,Y @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint16_t operand = vm.debuggerRead( pc+1 ) | (vm.debuggerRead( pc+2 ) << 8) ;
				addrSym[0] = (operand >>12) & 0xf;
				addrSym[1] = (operand >> 8) & 0xf;
				addrSym[2] = (operand >> 4) & 0xf;
				addrSym[3] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymY, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;

				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::Indirect: { //($xxxx) @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint16_t operand = vm.debuggerRead( pc+1 ) | (vm.debuggerRead( pc+2 ) << 8) ;
				addrSym[0] = (operand >>12) & 0xf;
				addrSym[1] = (operand >> 8) & 0xf;
				addrSym[2] = (operand >> 4) & 0xf;
				addrSym[3] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::IndirectX: { //($xxxx,X) @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint16_t operand = vm.debuggerRead( pc+1 ) | (vm.debuggerRead( pc+2 ) << 8) ;
				addrSym[0] = (operand >>12) & 0xf;
				addrSym[1] = (operand >> 8) & 0xf;
				addrSym[2] = (operand >> 4) & 0xf;
				addrSym[3] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymX, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::IndirectY: { //($xxxx),Y @ $xxxx(=xx)
				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				uint16_t operand = vm.debuggerRead( pc+1 ) | (vm.debuggerRead( pc+2 ) << 8) ;
				addrSym[0] = (operand >>12) & 0xf;
				addrSym[1] = (operand >> 8) & 0xf;
				addrSym[2] = (operand >> 4) & 0xf;
				addrSym[3] = (operand >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Comma, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), SymY, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), AtMark, 0.0f).width();
				startX += spaceSize;
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >> 12) & 0xf;
				addrSym[1] = (inst.addr_ >>  8) & 0xf;
				addrSym[2] = (inst.addr_ >>  4) & 0xf;
				addrSym[3] = (inst.addr_ >>  0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();

				startX += symRenderer_.renderSym(cv, Point(startX,offset), RightParen, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Eq, 0.0f).width();
				uint8_t val = vm.debuggerRead( inst.addr_ );
				numSym[0] = (val >> 4) & 0xf;
				numSym[1] = (val >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), numSym, 0.0f).width();
				startX += symRenderer_.renderSym(cv, Point(startX,offset), LeftParen, 0.0f).width();
			}
				break;
			case AddrMode::Relative: { //$xxxx
				startX += symRenderer_.renderSym(cv, Point(startX,offset), Dollar, 0.0f).width();
				addrSym[0] = (inst.addr_ >>12) & 0xf;
				addrSym[1] = (inst.addr_ >> 8) & 0xf;
				addrSym[2] = (inst.addr_ >> 4) & 0xf;
				addrSym[3] = (inst.addr_ >> 0) & 0xf;
				startX += this->numRenderer_.renderSyms(cv, Point(startX,offset), addrSym, 0.0f).width();
			}
				break;
			case AddrMode::None:
				break;
			default:
				break;
			}
			offset += rowHeight;
		}
	}
}

void NesTraceWidget::idle(const float delta_ms)
{
}

void NesTraceWidget::reshape(chisa::geom::Box const& areaSize)
{

}

chisa::geom::Box NesTraceWidget::measure(chisa::geom::Box const& constraintSize)
{
	//$xxxx aa bb cc LDA $xxxx(=0xdddd)
	// 33文字
	return chisa::geom::Box(this->numRenderer_.maxCharWidth()*8.0f, constraintSize.height());
}

}
