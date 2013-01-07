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

	this->symRenderer_.registerSymbol(Space         ," " );
	this->symRenderer_.registerSymbol(Dollar        ,"$" );
	this->symRenderer_.registerSymbol(Sharp         ,"#" );
	this->symRenderer_.registerSymbol(RightParen    ,"(" );
	this->symRenderer_.registerSymbol(LeftParen     ,")" );
	this->symRenderer_.registerSymbol(RightBracket  ,"<" );
	this->symRenderer_.registerSymbol(LeftBracket   ,">" );
	this->symRenderer_.registerSymbol(RightSqBracket,"[" );
	this->symRenderer_.registerSymbol(LeftSqBracket ,"]" );
	this->symRenderer_.registerSymbol(HexStart      ,"0x");
	this->symRenderer_.registerSymbol(Eq            ,"=" );
}

void NesTraceWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
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
	return chisa::geom::Box(this->asmRenderer_.maxCharWidth()*33.0f, chisa::geom::Unspecified);
}

}
