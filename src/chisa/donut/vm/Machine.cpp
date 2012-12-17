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
#include "../object/StringObject.h"
#include "../object/FloatObject.h"
#include "../object/NativeClosureObject.h"
#include "../object/DonutClosureObject.h"

#include <algorithm>

namespace chisa {
namespace donut {

const static std::string TAG("Machine");

Context::Context(Handler<Clock> const& clk)
:time_(clk->now())
,stack_()
,callStack_()
{

}

Context::Context(Handler<Clock> const& clk, Context const& other)
:time_(clk->now())
,stack_(other.stack_)
,callStack_(other.callStack_)
{

}


Machine::Machine(logging::Logger& log, Handler<Clock> const& clock, Handler<Heap> const& heap)
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

Handler<Object> Machine::start( Handler<Source> const& src )
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

Handler<Object> Machine::startContinue(Handler<Object> const& obj)
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

void Machine::enterClosure(Handler<Object> const& self, Handler<DonutClosureObject> const& clos, Handler<Object> const& args)
{
	Handler<DonutObject> scope ( heap_->createEmptyDonutObject() );
	scope->set(heap_, "__scope__", clos);
	{
		Handler<Closure> c = clos->closureCode();
		const std::size_t max = c->arglist().size();
		for(std::size_t i=0;i<max;++i){
			const std::string arg = c->arglist().at(i);
			scope->set( heap_, arg, args->get(heap_, i) );
		}
	}
	this->callStack().push_back( Callchain(0, this->stack().size(), self, clos, scope) );
}

bool Machine::leaveClosure()
{
	this->callStack().pop_back();
	return !this->callStack().empty(); //まだ実行するコールスタックが存在する
}


void Machine::pushStack( Handler<Object> const& obj )
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
			Handler<Object> obj(*(this->stack().end()-constIndex-1));
			this->pushStack( obj );
			break;
		}
		case Inst::ReplaceCopy: {
			*(this->stack().end()-constIndex-1) = this->stack().back();
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
				if(obj->has(heap_, name)){
					this->pushStack( obj );
					found = true;
					break;
				}else if( obj->has(heap_, "__scope__") ){
					obj = obj->get(heap_, "__scope__");
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
			this->pushStack( destObj->set(heap_, nameObj->toString(heap_), storeObj) );
			break;
		}
		case Inst::LoadObj: {
			Handler<Object> nameObj = this->popStack();
			Handler<Object> destObj = this->popStack();

			this->pushStack( destObj->get(heap_, nameObj->toString(heap_)) );
			break;
		}
		case Inst::Apply: {
			Handler<DonutObject> obj(heap_->createEmptyDonutObject());
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->popStack();
				obj->set(heap_, i-1, val);
			}

			Handler<Object> closureObj = this->popStack();
			Handler<Object> destObj = this->popStack();

			if( Handler<NativeClosureObject> nclos = closureObj->tryCastToNativeClosureObject() ){
				this->pushStack( nclos->apply(heap_, destObj, obj) );
			}else if( Handler<DonutClosureObject> dclos = closureObj->tryCastToDonutClosureObject() ){
				this->enterClosure(destObj, dclos, obj);
			}else{
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. \"%s\" is not callable.", closureObj->repr(heap_).c_str());
			}
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(heap_->createEmptyDonutObject() );
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->popStack();

				obj->set(heap_, util::toString(i-1), val);
			}
			this->pushStack(obj);
			break;
		}
		case Inst::ConstructObject: {
			Handler<Object> obj(heap_->createEmptyDonutObject() );
			for(int i=0;i<constIndex;++i){
				Handler<Object> val = this->popStack();
				Handler<Object> name = this->popStack();
				obj->set(heap_, name->toString(heap_), val);
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
			throw DonutException(__FILE__, __LINE__,
					"[BUG] Oops. Unknwon opcode: closure<%s>@%08x=%08x => (%02x,%02x,%04x)",
					closureObject()->repr(heap_).c_str(), this->pc()-1, inst,opcode,constKind,constIndex);
		}
	}
	Handler<Object> result(this->popStack());
	if( !this->isInterrupted() && !this->stack().empty() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Execution ended, but stack id not empty:%d", this->stack().size());
	}
	return result;
}

/**********************************************************************************
 * save/load
 **********************************************************************************/
void Machine::bootstrap()
{
	this->contextRevs_.clear();
}

util::XValue Callchain::save()
{
	using namespace chisa::util;
	Handler<util::XObject> top(new util::XObject);
	top->set("pc", this->pc_);
	top->set("stack_base", this->stackBase_);
	top->set("self", this->self_->toDescriptor());
	top->set("closure", this->closure_->toDescriptor());
	top->set("scope", this->scope_->toDescriptor());
	return top;
}

Callchain::Callchain(Handler<Heap> const& heap, util::XValue const& data)
{
	using namespace chisa::util;
	Handler<util::XObject> obj( data.as<XObject>() );
	this->pc_ = obj->get<decltype(this->pc_)>("pc");
	this->stackBase_ = obj->get<decltype(this->stackBase_)>("stack_base");
	this->self_ = heap->decodeDescriptor( obj->get<object_desc_t>("self") );
	this->closure_ = heap->decodeDescriptor( obj->get<object_desc_t>("closure") ).cast<DonutClosureObject>();
	this->scope_ = heap->decodeDescriptor( obj->get<object_desc_t>("scope") ).cast<DonutObject>();
}

util::XValue Context::save()
{
	using namespace chisa::util;
	Handler<util::XObject> top(new util::XObject);
	// time
	top->set("time", this->time_);
	{ // callstack
		Handler<util::XArray> list(new util::XArray);
		for( Callchain& chain : this->callStack_ ) {
			list->append(chain.save());
		}
		top->set("callstack", list);
	}
	{ //stack
		Handler<util::XArray> list(new util::XArray);
		for( Handler<Object> const& obj : this->stack_ ) {
			list->append( obj->toDescriptor() );
		}
		top->set("stack", list);
	}
	return top;
}
Context::Context(Handler<Heap> const& heap, util::XValue const& data)
{
	using namespace chisa::util;
	Handler<XObject> obj ( data.as<XObject>() );
	{ //time
		this->time_ = obj->get<unsigned int>("time");
	}
	{ // callstack
		for( XValue& e : *(obj->get<XArray>("callstack"))) {
			this->callStack_.push_back( Callchain(heap, e) );
		}
	}
	{ //stack
		for( XValue& e : *(obj->get<XArray>("stack"))) {
			this->stack_.push_back( heap->decodeDescriptor(e.as<object_desc_t>()) );
		}
	}
	//ローカルは再利用されない前提なので保存しない
}

util::XValue Machine::save()
{
	Handler<util::XArray> top(new util::XArray);
	//context_revsさえ保存すればよい
	for( Context& ctx : this->contextRevs_ ) {
		top->append( ctx.save() );
	}
	return top;
}
void Machine::load( util::XValue const& obj)
{
	for(auto data : *(obj.as<util::XArray>())){
		this->contextRevs_.push_back( Context(heap_, data) );
	}
	if( !std::is_sorted(this->contextRevs_.begin(), this->contextRevs_.end(), Context::CompareByTime()) ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Invalid save data. Revision data is not sorted.");
	}
}

/**********************************************************************************
 * time functions
 **********************************************************************************/

// 特に何もしない　問題ないはず
// 実行時にdiscardFuture/discardHistoryされるので実行時に駄目になることはないはず
void Machine::onBackNotify(){}
void Machine::onForwardNotify(){}

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

