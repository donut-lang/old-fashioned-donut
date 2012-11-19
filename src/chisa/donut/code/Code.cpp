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

#include <cstddef>
#include "Code.h"
#include "Closure.h"
#include "..//Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

Source::Source()
{
	// TODO Auto-generated constructor stub

}

std::string Source::disasm( Instruction inst )
{
	Instruction opcode;
	Instruction constKind;
	int constIndex;
	this->disasm(inst, opcode, constKind, constIndex);

	std::string repl=util::format("#%08x ", inst);

	switch(opcode) {
	case Nop:
		repl += "Nop";
		break;
	case Push:
		repl += "Push";
		break;
	case PushCopy:
		repl += "PushCopy";
		break;
	case Pop:
		repl += "Pop";
		break;
	case SearchScope:
		repl += "SearchScope";
		break;
	case StoreObj:
		repl += "StoreObj";
		break;
	case LoadObj:
		repl += "LoadObj";
		break;
	case LoadLocal:
		repl += "LoadLocal";
		break;
	case StoreLocal:
		repl += "StoreLocal";
		break;
	case Apply:
		repl += "Apply";
		break;
	case ConstructArray:
		repl += "ConstuctArray";
		break;
	case ConstructObject:
		repl += "ConstuctObject";
		break;
	case Branch:
		repl += "Branch";
		break;
	case BranchTrue:
		repl += "BranchTrue";
		break;
	case BranchFalse:
		repl += "BranchFalse";
		break;
	default:
		repl += "????????";
		break;
	}

	while(repl.size() < 22){
		repl += " ";
	}

	switch(constKind) {
	case Inst::ConstOperand:
		repl += util::format("%d", constIndex);
		break;
	case Inst::ConstInt:
		repl += util::format("int:%d -> %d", constIndex, this->intTable_.get(constIndex));
		break;
	case Inst::ConstBool:
		repl += util::format("bool:%d -> %s", constIndex, constIndex == 0 ? "false" : "true");
		break;
	case Inst::ConstFloat:
		repl += util::format("float:%d -> %f", constIndex, this->floatTable_.get(constIndex));
		break;
	case Inst::ConstClosure:
		repl += util::format("closure:%d -> %d", constIndex, constIndex); //TODO
		break;
	case Inst::ConstString:
		repl += util::format("string:%d -> %s", constIndex, this->stringTable_.get(constIndex).c_str());
		break;
	case Inst::ConstNull:
		repl += util::format("null");
		break;
	default:
		repl += "????????";
		break;
	}
	return repl;
}


template<>
Instruction Source::constCode<bool>(bool const& val)
{
	return Inst::ConstBool | (val ? 1 : 0);
}

template<>
Instruction Source::constCode<int>(int const& val)
{
	return Inst::ConstInt | this->intTable_.regist(val);
}

template<>
Instruction Source::constCode<float>(float const& val)
{
	return Inst::ConstFloat | this->floatTable_.regist(val);
}

template<>
Instruction Source::constCode<Handler<donut::Closure> >(Handler<donut::Closure> const& val)
{
	return Inst::ConstClosure | this->closureTable_.regist(val);
}

template<>
Instruction Source::constCode<std::nullptr_t>(std::nullptr_t const& null)
{
	return Inst::ConstNull;
}

template<>
Instruction Source::constCode<std::string>(std::string const& val)
{
	return Inst::ConstString | this->stringTable_.regist(val);
}

}}
