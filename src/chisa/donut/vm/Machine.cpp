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

#include "Machine.h"

namespace chisa {
namespace donut {

Machine::Machine(World* pool)
:pc_(0),world_(pool)
{
	// TODO Auto-generated constructor stub

}

void Machine::enterClosure(Handler<Closure> clos)
{

}

void Machine::run()
{
	Handler<Code> code = this->world_->code();
	Handler<ClosureObject> closObj(this->callStack_.back());
	std::vector<Instruction> const& asmlist(closObj->closure()->instlist());
	while(1){
		Instruction inst = asmlist[this->pc_++];
		Instruction opcode, constKind, constIndex;
		code->disasm(inst, opcode, constKind, constIndex);
		switch(opcode){
		default:
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closObj->toString(world_).c_str(), this->pc_-1);
		}
	}
}

}}

