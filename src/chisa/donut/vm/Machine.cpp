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
#include "../object/NativeObject.h"
#include "../object/DonutObject.h"

namespace chisa {
namespace donut {

const static std::string TAG("Machine");

Machine::Machine(logging::Logger& log, World* world)
:log_(log)
,world_(world)
,pc_(0)
,asmlist_(nullptr)
,local_(32)
{
}

Handler<Object> Machine::start( const std::size_t closureIndex )
{
	this->enterClosure(world_->createNull(), createClosure( world_->code()->getClosure( closureIndex ) ), world_->createNull());
	return this->run();
}

Handler<DonutClosureObject> Machine::createClosure(Handler<Closure> closureCode)
{
	if( this->context_ ){
		return world_->createDonutClosureObject(closureCode, this->context_);
	}else{
		return world_->createDonutClosureObject(closureCode, world_->createNull());
	}
}

void Machine::enterClosure(Handler<Object> self, Handler<DonutClosureObject> clos, Handler<Object> args)
{
	if(this->closure_){
		this->callStack_.push_back( Callchain(this->pc_, this->self_, this->closure_, this->context_) );
	}
	this->context_ = world_->createEmptyDonutObject();
	this->context_->store(world_, "__scope__", clos);
	this->self_ = self;
	this->closure_ = clos;
	this->pc_ = 0;
	this->asmlist_ = &(clos->closureCode()->instlist());
	{
		Handler<Closure> c = clos->closureCode();
		const std::size_t max = c->arglist().size();
		for(std::size_t i=0;i<max;++i){
			const std::string arg = c->arglist().at(i);
			this->context_->store( world_, arg, args->load(world_, i) );
		}
	}
}

bool Machine::returnClosure()
{
	if(this->callStack_.empty()){
		return false;
	}
	Callchain& chain = this->callStack_.back();
	this->closure_ = chain.closure_;
	this->pc_ = chain.pc_;
	this->self_ = chain.self_;
	this->context_ = chain.context_;
	this->callStack_.pop_back();

	this->asmlist_ = &( closure_->closureCode()->instlist() );
	return true;
}
Handler<Object> Machine::run()
{
	Handler<Code> const code = this->world_->code();
	bool running = true;
	while( running ){
		if( this->pc_ >= asmlist_->size() ){
			running &= this->returnClosure();
			continue;
		}
		Instruction const inst((*asmlist_)[this->pc_++]);
		Instruction opcode, constKind;
		int constIndex;
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
				this->stack_.push_back( world_->createBool( code->getBool(constIndex) ) );
				break;
			}
			case Inst::ConstFloat: {
				this->stack_.push_back( world_->createFloatObject( code->getFloat(constIndex) ) );
				break;
			}
			case Inst::ConstClosure: {
				this->stack_.push_back( createClosure( code->getClosure(constIndex) ) );
				break;
			}
			case Inst::ConstInt: {
				this->stack_.push_back( world_->createInt( code->getInt(constIndex) ) );
				break;
			}
			case Inst::ConstString: {
				this->stack_.push_back( world_->createStringObject( code->getString(constIndex) ) );
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
			const std::string name = nameObj->toString(world_);
			this->stack_.pop_back();

			bool found = false;
			Handler<Object> obj = this->context_;
			while(!found){
				if(obj->have(world_, name)){
					this->stack_.push_back( obj );
					found = true;
					break;
				}else if( obj->have(world_, "__scope__") ){
					obj = obj->load(world_, "__scope__");
				}else{
					break;
				}
			}
			if(!found){
				this->stack_.push_back( this->context_ );
			}
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
			Handler<DonutObject> obj(world_->createEmptyDonutObject());
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, i-1, val);
			}

			Handler<Object> closureObj = this->stack_.back();
			this->stack_.pop_back();

			Handler<Object> destObj = this->stack_.back();
			this->stack_.pop_back();

			//XXX: ちゃんと型を使う
			if(!closureObj->isObject()){
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(world_).c_str());
			} else if ( Handler<DonutClosureObject> closObj = closureObj.tryCast<DonutClosureObject>() ) {
				this->enterClosure(destObj, closObj, obj);
			} else if ( Handler<PureNativeClosureObject> clos = closureObj.tryCast<PureNativeClosureObject>() ) {
				this->stack_.push_back( clos->apply(destObj, obj) );
			}else{
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(world_).c_str());
			}
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(world_->createEmptyDonutObject() );
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, util::toString(i-1), val);
			}
			this->stack_.push_back(obj);
			break;
		}
		case Inst::ConstructObject: {
			Handler<Object> obj(world_->createEmptyDonutObject() );
			for(int i=0;i<constIndex;++i){
				Handler<Object> val = this->stack_.back();
				this->stack_.pop_back();
				Handler<Object> name = this->stack_.back();
				this->stack_.pop_back();
				obj->store(world_, name->toString(world_), val);
			}
			this->stack_.push_back(obj);
			break;
		}
		case Inst::Branch: {
			this->pc_ += constIndex;
			break;
		}
		case Inst::BranchTrue: {
			Handler<Object> val = this->stack_.back();
			this->stack_.pop_back();
			if(val->toBool(world_)){
				this->pc_ += constIndex;
			}
			break;
		}
		case Inst::BranchFalse: {
			Handler<Object> val = this->stack_.back();
			this->stack_.pop_back();
			if(!val->toBool(world_)){
				this->pc_ += constIndex;
			}
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

