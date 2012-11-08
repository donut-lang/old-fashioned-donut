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
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

const static std::string TAG("Machine");

Machine::Machine(logging::Logger& log, World* world)
:log_(log)
,pc_(0)
,local_(32)
,world_(world)
{
}

Handler<Object> Machine::start( const std::size_t closureIndex )
{
	this->closure_ = world_->create<ClosureObject>(world_->code()->getClosure(closureIndex));
	this->pc_ = 0;
	return this->run();
}

void Machine::enterClosure(Handler<ClosureObject> clos)
{
	if(this->closure_){
		this->callStack_.push_back( std::pair<Handler<ClosureObject>, pc_t >(this->closure_,this->pc_) );
	}
	this->closure_ = clos;
	this->pc_ = 0;
}

void Machine::returnClosure()
{
	std::pair<Handler<ClosureObject>, pc_t > st = this->callStack_.back();
	this->callStack_.pop_back();
	this->closure_ = st.first;
	this->pc_ = st.second;
}
Handler<Object> Machine::run()
{
	Handler<Code> code = this->world_->code();
	std::vector<Instruction> const& asmlist(closure_->closure()->instlist());
	while( this->pc_ < asmlist.size() ){
		Instruction const inst(asmlist[this->pc_++]);
		Instruction opcode, constKind, constIndex;
		code->disasm(inst, opcode, constKind, constIndex);
		if(this->log().t()){
			this->log().t(TAG, code->disasm(inst));
		}
		switch(opcode){
		case Inst::Nop:
			break;
		case Inst::Push: {
			switch(constKind){
			case Inst::ConstBool: {
				this->stack_.push_back( world_->createBool( code->getInt(constIndex) ) );
				break;
			}
			case Inst::ConstFloat: {
				this->stack_.push_back( world_->create<FloatObject>( code->getFloat(constIndex) ) );
				break;
			}
			case Inst::ConstClosure: {
				this->stack_.push_back( world_->create<ClosureObject>( code->getClosure(constIndex) ) );
				break;
			}
			case Inst::ConstInt: {
				this->stack_.push_back( world_->createInt( code->getInt(constIndex) ) );
				break;
			}
			case Inst::ConstString: {
				this->stack_.push_back( world_->create<StringObject>( code->getString(constIndex) ) );
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
		case Inst::SearchScope: {
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();
			this->stack_.push_back( this->closure_->searchScope(nameObj->toString(world_)) );
			break;
		}
		case Inst::StoreObj: {
			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> storeObj = this->stack_.back();
			this->stack_.pop_back();
			this->stack_.push_back( destObj->store(world_, nameObj->toString(world_), storeObj) );
			break;
		}
		case Inst::LoadObj: {
			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();
			this->stack_.push_back( destObj->load(world_, nameObj->toString(world_)) );
			break;
		}
		case Inst::LoadLocal: {
			Handler<Object> obj = this->local_[constIndex&31];
			this->stack_.push_back(obj);
			this->local_[constIndex&31];
			break;
		}
		case Inst::StoreLocal: {
			Handler<Object> obj = this->stack_.back();
			this->local_[constIndex&31] = obj;
			break;
		}
		case Inst::Apply: {
			Handler<Object> closureObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> selfObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(world_->create<BaseObject>());
			for(unsigned int i=0;i<constIndex;++i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, util::toString(i), val);
			}
			this->stack_.push_back(obj);
			break;
		}
		case Inst::ConstructObject: {
			Handler<Object> obj(world_->create<BaseObject>());
			for(unsigned int i=0;i<constIndex;++i){
				Handler<Object> name = this->stack_.back();
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, name->toString(world_), val);
			}
			break;
		}
		default:
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closure_->toString(world_).c_str(), this->pc_-1);
		}
	}
	Handler<Object> result(this->stack_.back());
	this->stack_.pop_back();
	return result;
}

}}

