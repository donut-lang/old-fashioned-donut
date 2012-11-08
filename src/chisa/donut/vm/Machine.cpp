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
#include "../Exception.h"
namespace chisa {
namespace donut {

Machine::Machine(logging::Logger& log, World* world)
:log_(log),pc_(0),world_(world)
{

}

void Machine::start( const std::size_t closureIndex )
{
	this->enterClosure( world_->code()->getClosure(closureIndex) );
	this->run();
}

void Machine::enterClosure(Handler<Closure> clos)
{
	if(this->closure_){
		this->callStack_.push_back( std::pair<Handler<ClosureObject>, pc_t >(this->closure_,this->pc_) );
	}
	this->closure_ = world_->create<ClosureObject>(clos);
	this->pc_ = 0;
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
	while( this->pc_ < asmlist.size() ){
		Instruction const inst(asmlist[this->pc_++]);
		Instruction opcode, constKind, constIndex;
		code->disasm(inst, opcode, constKind, constIndex);
		switch(opcode){
		case Inst::Nop:
			break;
		case Inst::Push: {
			switch(constKind){
			case Inst::ConstBool: {
				this->stack_.push_back( world_->createBool( code->getInt(constIndex) ) );
			}
			case Inst::ConstFloat:
			case Inst::ConstClosure: {
				this->stack_.push_back( world_->create<ClosureObject>( code->getClosure(constIndex) ) );
			}
			case Inst::ConstInt: {
				this->stack_.push_back( world_->createInt( code->getInt(constIndex) ) );
			}
			case Inst::ConstString: {
				break;
			}
			default:
				throw DonutException(__FILE__, __LINE__, "Unknwon const type: %d", constKind);
			}
			break;
		}
		case Inst::PushCopy: {
			Handler<Object> obj(this->stack_.back());
			this->stack_.push_back(obj);
			break;
		}
		case Inst::Pop:
			this->stack_.pop_back();
			break;
		case Inst::SearchScope:
			break;
		case Inst::StoreObj:
			break;
		case Inst::LoadObj:
			break;
		case Inst::LoadLocal:
			break;
		case Inst::StoreLocal:
			break;
		case Inst::Apply:
			break;
		case Inst::ConstructArray:
			break;
		case Inst::ConstructObject:
			break;
		default:
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closure_->toString(world_).c_str(), this->pc_-1);
		}
	}
}

}}

