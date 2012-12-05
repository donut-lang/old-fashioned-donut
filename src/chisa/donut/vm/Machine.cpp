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
#include "../source/Source.h"

namespace chisa {
namespace donut {

const static std::string TAG("Machine");

Context::Context(const Handler<Clock>& clk)
:time_(clk->now())
,stack_()
,local_(32)
,callStack_()
{

}

Context::Context(const Handler<Clock>& clk, const Context& other)
:time_(clk->now())
,stack_(other.stack_)
,local_(other.local_)
,callStack_(other.callStack_)
{

}


Machine::Machine(logging::Logger& log, const Handler<Clock>& clock, const Handler<Heap>& heap)
:log_(log)
,clock_(clock)
,heap_(heap)
{
}

Handler<Object> const& Machine::self()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.self_;
}

Handler<DonutObject> const& Machine::scope()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.scope_;
}

Handler<DonutClosureObject> const& Machine::closureObject()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.closure_;
}

Handler<Source> const& Machine::src()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	Handler<DonutClosureObject> const& clos = chain.closure_;
	return clos->source();
}

Handler<Closure> const& Machine::closure()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	Handler<DonutClosureObject> const& clos = chain.closure_;
	return clos->closureCode();
}

pc_t& Machine::pc()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.pc_;
}

bool Machine::fetchPC( Instruction& inst )
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	std::vector<donut::Instruction> const& instList = chain.closure_->closureCode()->instlist();
	if( chain.pc_ >= instList.size() ){
		return false;
	}
	inst = instList.at(chain.pc_++);
	return true;
}

std::vector<Callchain>& Machine::callStack()
{
	Context& ctx =  this->contextRevs_.back();
	return ctx.callStack_;
}

std::vector<Handler<Object> >& Machine::local()
{
	Context& ctx =  this->contextRevs_.back();
	return ctx.local_;
}

std::vector<Handler<Object> >& Machine::stack()
{
	Context& ctx =  this->contextRevs_.back();
	return ctx.stack_;
}

unsigned int Machine::stackBase()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.stackBase_;
}

Handler<Object> Machine::start( const Handler<Source>& src )
{
	if( this->isInterrupted() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. This machine is interrupted now. Call #startContinue instead.");
	}
	this->clock_->tick();
	if( this->contextRevs_.empty() ){
		this->contextRevs_.push_back( Context( this->clock_ ) );
	}else{
		this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	}
	Handler<DonutClosureObject> entryPoint( heap_->createDonutClosureObject(src, src->getEntrypointID(), heap_->global()) );
	this->enterClosure(heap_->createNull(), entryPoint, heap_->createNull());
	return this->run();
}

Handler<Object> Machine::startContinue(const Handler<Object>& obj)
{
	if( !this->isInterrupted() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. This machine is not interrupted now. Call #start instead.");
	}
	this->clock_->tick();
	this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	this->pushStack(obj);
	return this->run();
}

bool Machine::isInterrupted() const noexcept
{
	if( this->contextRevs_.empty() ) {
		return false;
	}
	unsigned int const time = this->clock_->now();
	// シークされてない
	Context const& last = this->contextRevs_.back();
	if( time == last.time_ ){
		return !last.callStack_.empty();
	}
	// シークされてるので、インデックスを探す
	int idx = -1;
	for(int i=this->contextRevs_.size()-1; i>=0;--i){
		Context const& c = this->contextRevs_[i];
		if(time >= c.time_){
			idx = i;
			break;
		}
	}
	if( idx < 0 ){
		return false;
	}
	Context const& ctx = this->contextRevs_[idx];
	return !ctx.callStack_.empty();
}

void Machine::enterClosure(const Handler<Object>& self, const Handler<DonutClosureObject>& clos, const Handler<Object>& args)
{
	Handler<DonutObject> scope ( heap_->createEmptyDonutObject() );
	scope->store(heap_, "__scope__", clos);
	{
		Handler<Closure> c = clos->closureCode();
		const std::size_t max = c->arglist().size();
		for(std::size_t i=0;i<max;++i){
			const std::string arg = c->arglist().at(i);
			scope->store( heap_, arg, args->load(heap_, i) );
		}
	}
	this->callStack().push_back( Callchain(0, this->stack().size(), self, clos, scope) );
}

bool Machine::leaveClosure()
{
	this->callStack().pop_back();
	return !this->callStack().empty(); //まだ実行するコールスタックが存在する
}


void Machine::pushStack( const Handler<Object>& obj )
{
	this->stack().push_back( obj );
}
Handler<Object> Machine::popStack()
{
	std::vector<Handler<Object> >& stack = this->stack();
	Handler<Object> obj( stack.back() );
	stack.pop_back();
	return obj;
}
Handler<Object> Machine::topStack()
{
	return this->stack().back();
}

Handler<Object> Machine::run()
{
	bool running = true;
	Instruction inst;
	while( running ){
		if(!this->fetchPC( inst )){ //このクロージャの終端に来ました
			running &= this->leaveClosure();
			continue;
		}
		Instruction opcode, constKind;
		int constIndex;
		Source::disasm(inst, opcode, constKind, constIndex);
		if(this->log().t()){
			this->log().t(TAG, util::format("$%04x ", this->pc())+this->src()->disasm(inst));
		}
		switch(opcode){
		case Inst::Nop:
			break;
		case Inst::Push: {
			switch(constKind){
			case Inst::ConstBool: {
				this->pushStack( this->heap_->createBool( this->src()->getBool( constIndex ) ) );
				break;
			}
			case Inst::ConstFloat: {
				this->pushStack( this->heap_->createFloatObject( this->src()->getFloat( constIndex ) ) );
				break;
			}
			case Inst::ConstClosure: {
				this->pushStack( this->heap_->createDonutClosureObject(this->src(), constIndex, this->scope()) );
				break;
			}
			case Inst::ConstInt: {
				this->pushStack( this->heap_->createInt( this->src()->getInt( constIndex ) ) );
				break;
			}
			case Inst::ConstString: {
				this->pushStack( this->heap_->createStringObject( this->src()->getString( constIndex ) ) );
				break;
			}
			case Inst::ConstNull: {
				this->pushStack( this->heap_->createNull() );
				break;
			}
			default:
				throw DonutException(__FILE__, __LINE__, "Unknwon const type: %d", constKind);
			}
			break;
		}
		case Inst::PushCopy: {
			this->pushStack( this->topStack() );
			break;
		}
		case Inst::Pop:
			this->popStack();
			break;
		case Inst::SearchScope: {
			Handler<Object> nameObj = this->popStack();
			const std::string name = nameObj->toString(heap_);

			bool found = false;
			Handler<Object> obj = this->scope();
			while(!found){
				if(obj->have(heap_, name)){
					this->pushStack( obj );
					found = true;
					break;
				}else if( obj->have(heap_, "__scope__") ){
					obj = obj->load(heap_, "__scope__");
				}else{
					break;
				}
			}
			if(!found){
				this->pushStack( this->scope() );
			}
			break;
		}
		case Inst::StoreObj: {
			Handler<Object> storeObj = this->popStack();
			Handler<Object> nameObj = this->popStack();
			Handler<Object> destObj = this->popStack();
			this->pushStack( destObj->store(heap_, nameObj->toString(heap_), storeObj) );
			break;
		}
		case Inst::LoadObj: {
			Handler<Object> nameObj = this->popStack();
			Handler<Object> destObj = this->popStack();

			this->pushStack( destObj->load(heap_, nameObj->toString(heap_)) );
			break;
		}
		case Inst::LoadLocal: {
			Handler<Object> obj = this->local()[constIndex&31];
			this->pushStack(obj);
			break;
		}
		case Inst::StoreLocal: {
			this->local()[constIndex&31] = this->topStack();
			break;
		}
		case Inst::Apply: {
			Handler<DonutObject> obj(heap_->createEmptyDonutObject());
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->popStack();
				obj->store(heap_, i-1, val);
			}

			Handler<Object> closureObj = this->popStack();
			Handler<Object> destObj = this->popStack();

			//XXX: ちゃんと型を使う
			if(!closureObj->isObject()){
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(heap_).c_str());
			} else if ( Handler<DonutClosureObject> closObj = closureObj.tryCast<DonutClosureObject>() ) {
				this->enterClosure(destObj, closObj, obj);
			} else if ( Handler<PureNativeClosureObject> clos = closureObj.tryCast<PureNativeClosureObject>() ) {
				this->pushStack( clos->apply(heap_, destObj, obj) );
			}else{
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->toString(heap_).c_str());
			}
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(heap_->createEmptyDonutObject() );
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->popStack();

				obj->store(heap_, util::toString(i-1), val);
			}
			this->pushStack(obj);
			break;
		}
		case Inst::ConstructObject: {
			Handler<Object> obj(heap_->createEmptyDonutObject() );
			for(int i=0;i<constIndex;++i){
				Handler<Object> val = this->popStack();
				Handler<Object> name = this->popStack();
				obj->store(heap_, name->toString(heap_), val);
			}
			this->pushStack(obj);
			break;
		}
		case Inst::Branch: {
			this->pc() += constIndex;
			break;
		}
		case Inst::BranchTrue: {
			Handler<Object> val = this->popStack();
			if(val->toBool(heap_)){
				this->pc() += constIndex;
			}
			break;
		}
		case Inst::BranchFalse: {
			Handler<Object> val = this->popStack();
			if(!val->toBool(heap_)){
				this->pc() += constIndex;
			}
			break;
		}
		case Inst::PushSelf: {
			this->pushStack( this->self() );
			break;
		}
		case Inst::Interrupt: {
			running = false;
			break;
		}
		case Inst::Return: {
			this->stack().resize( this->stackBase()+1 );
			running &= this->leaveClosure();
			break;
		}
		default:
			throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Unknwon opcode: closure<%s>:%08x", closureObject()->toString(heap_).c_str(), this->pc()-1);
		}
	}
	Handler<Object> result(this->popStack());
	if( !this->isInterrupted() && !this->stack().empty() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Execution ended, but stack id not empty:%d", this->stack().size());
	}
	return result;
}

/**********************************************************************************
 * from clock
 **********************************************************************************/

void Machine::onSeekNotify()
{
	// 特に何もしない　問題ないはず
	// 実行時にdiscardFuture/discardHistoryされるので実行時に駄目になることはないはず
}

/**
 * 現在の時刻以降のコンテキストを消し去る。
 * これが実際に必要なのは、ヒープを共有する他のマシンがシークされた後に動き出した時。
 *  あくまでクロックからのみ呼ばれる。自分からは呼び出さない。
 */
void Machine::onDiscardFutureNotify()
{
	timestamp_t const time = clock_->now();
	int idx = -1;
	for(int i=this->contextRevs_.size()-1; i>=0;--i){
		Context& c = this->contextRevs_[i];
		if(time >= c.time_){
			idx = i;
			break;
		}
	}
	this->contextRevs_.erase( this->contextRevs_.begin()+idx+1, this->contextRevs_.end() );
}

/**
 * 現在の時刻以前のコンテキストを消し去ってしまう。
 *  あくまでクロックからのみ呼ばれる。自分からは呼び出さない。
 */
void Machine::onDiscardHistoryNotify()
{
	timestamp_t const time = clock_->now();
	int idx = this->contextRevs_.size();
	const int max = this->contextRevs_.size();
	for(int i=0; i<max;++i){
		Context& c = this->contextRevs_[i];
		if( c.time_ > time ){
			idx = i;
			break;
		}
	}
	this->contextRevs_.erase( this->contextRevs_.begin(), this->contextRevs_.begin()+idx );
}

}}

