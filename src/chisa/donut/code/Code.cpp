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

#include "Code.h"
#include "Closure.h"
#include "../../logging/Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

Code::Code()
{
	// TODO Auto-generated constructor stub

}

std::string Code::disasm( Instruction inst )
{
	Instruction opcode;
	Instruction constKind;
	Instruction constIndex;
	this->disasm(inst, opcode, constKind, constIndex);

	std::string repl=util::format("#%02x ", inst);

	switch(opcode) {

	}

	repl += " ";

	switch(constKind) {
	case Inst::ConstOperand:
		repl += util::format("<%d>", constIndex);
		break;
	case Inst::ConstInt:
		repl += util::format("<int:%d> %d", constIndex, this->intTable_.get(constIndex));
		break;
	case Inst::ConstBool:
		repl += util::format("<bool:%d> %s, constIndex", constIndex == 0 ? "false" : "true");
		break;
	case Inst::ConstFloat:
		repl += util::format("<float:%d> %f", constIndex, this->floatTable_.get(constIndex));
		break;
	case Inst::ConstClosure:
		repl += util::format("<closure:%d> %d", constIndex, constIndex); //TODO
		break;
	case Inst::ConstString:
		repl += util::format("<string:%d> %s", constIndex, this->stringTable_.get(constIndex).c_str());
		break;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon const kind: %d", (constKind >> Inst::ConstKindShift));
	}
	return repl;
}

Handler<Closure> Code::getClosure(unsigned int index)
{
	index &= 0xffff;
	return this->closureTable_.get(index);
}

std::size_t Code::numClosure() const
{
	return this->closureTable_.size();
}

template<>
Instruction Code::constCode<bool>(bool const& val)
{
	return Inst::ConstBool | (val ? 1 : 0);
}

template<>
Instruction Code::constCode<int>(int const& val)
{
	return Inst::ConstInt | this->intTable_.regist(val);
}

template<>
Instruction Code::constCode<float>(float const& val)
{
	return Inst::ConstFloat | this->floatTable_.regist(val);
}

template<>
Instruction Code::constCode<Handler<donut::Closure> >(Handler<donut::Closure> const& val)
{
	return Inst::ConstClosure | this->closureTable_.regist(val);
}

template<>
Instruction Code::constCode<std::string>(std::string const& val)
{
	return Inst::ConstString | this->stringTable_.regist(val);
}

}}
