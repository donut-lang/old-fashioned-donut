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
,world_(world)
,pc_(0)
,local_(32)
{
}

Handler<Object> Machine::start( const std::size_t closureIndex )
{
	this->enterClosure( world_->createNull(), world_->create<ClosureObject>(world_->code()->getClosure(closureIndex)));
	return this->run();
}

void Machine::enterClosure(Handler<Object> self, Handler<ClosureObject> clos)
{
	if(this->closure_){
		this->callStack_.push_back( Callchain(this->pc_, this->self_, this->closure_) );
	}
	this->self_ = self;
	this->closure_ = clos;
	this->pc_ = 0;
	this->asmlist_ = &(clos->closure()->instlist());
}

void Machine::returnClosure()
{
	Callchain& chain = this->callStack_.back();
	this->closure_ = chain.closure_;
	this->pc_ = chain.pc_;
	this->self_ = chain.self_;
	this->callStack_.pop_back();
	this->asmlist_ = &(closure_->closure()->instlist());
}
Handler<Object> Machine::run()
{
	Handler<Code> const code = this->world_->code();
	while( this->pc_ < asmlist_->size() ){
		Instruction const inst((*asmlist_)[this->pc_++]);
		Instruction opcode, constKind, constIndex;
		code->disasm(inst, opcode, constKind, constIndex);
		if(this->log().t()){
			this->log().t(TAG, util::format("$%04x ", this->pc_)+code->disasm(inst));
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
				this->stack_.push_back( world_->create<ClosureObject>( code->getClosure(constIndex), this->closure_ ) );
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
			case Inst::ConstNull: {
				this->stack_.push_back( world_->createNull() );
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
			Handler<Object> storeObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();
			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();
			this->stack_.push_back( destObj->store(world_, nameObj->toString(world_), storeObj) );
			break;
		}
		case Inst::LoadObj: {
			Handler<Object> nameObj = this->stack_.back();
			this->stack_.pop_back();

			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();

			this->stack_.push_back( destObj->load(world_, nameObj->toString(world_)) );
			break;
		}
		case Inst::LoadLocal: {
			Handler<Object> obj = this->local_[constIndex&31];
			this->stack_.push_back(obj);
			break;
		}
		case Inst::StoreLocal: {
			Handler<Object> obj = this->stack_.back();
			this->local_[constIndex&31] = obj;
			break;
		}
		case Inst::Apply: {
			Handler<BaseObject> obj(world_->create<BaseObject>());
			for(unsigned int i=0;i<constIndex;++i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, util::toString(i), val);
			}

			Handler<Object> closureObj = this->stack_.back();
			this->stack_.pop_back();

			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();

			//XXX: ちゃんと型を使う
			if(!closureObj->isObject()){
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(world_).c_str());
			} else if ( Handler<ClosureObject> closObj = closureObj.tryCast<ClosureObject>() ) {
				this->enterClosure(destObj, closObj);
			} else if ( Handler<PureNativeClosure> builtin = closureObj.tryCast<PureNativeClosure>() ) {
				this->stack_.push_back( Handler<Object>::__internal__fromRawPointerWithoutCheck( builtin->apply(destObj.get(), obj.get()) ) );
			}else{
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(world_).c_str());
			}
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(world_->create<BaseObject>());
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, util::toString(i-1), val);
			}
			this->stack_.push_back(obj);
			break;
		}
		case Inst::ConstructObject: {
			Handler<Object> obj(world_->create<BaseObject>());
			for(unsigned int i=0;i<constIndex;++i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				Handler<Object> name = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, name->toString(world_), val);
			}
			this->stack_.push_back(obj);
			break;
		}
		default:
			throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closure_->toString(world_).c_str(), this->pc_-1);
		}
	}
	Handler<Object> result(this->stack_.back());
	this->stack_.pop_back();
	return result;
}

}}

