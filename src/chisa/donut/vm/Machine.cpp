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

}

void Machine::enterClosure(Handler<Closure> clos)
{
	if(this->closure_){
		this->callStack_.push_back( std::pair<Handler<ClosureObject>, pc_t >(this->closure_,this->pc_) );
	}
}

void Machine::returnClosure()
{
	std::pair<Handler<ClosureObject>, pc_t > st = this->callStack_.back();
	this->callStack_.pop_back();
	this->closure_ = st.first;
	this->pc_ = st.second;
}
void Machine::run()
{
	Handler<Code> code = this->world_->code();
	std::vector<Instruction> const& asmlist(closure_->closure()->instlist());
	while(1){
		Instruction inst = asmlist[this->pc_++];
		Instruction opcode, constKind, constIndex;
		code->disasm(inst, opcode, constKind, constIndex);
		switch(opcode){
		default:
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closure_->toString(world_).c_str(), this->pc_-1);
		}
	}
}

}}

