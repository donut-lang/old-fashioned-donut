/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include <cinamo/String.h>
#include <donut/vm/Machine.h>
#include <donut/Exception.h>
#include <donut/object/StringObject.h>
#include <donut/object/FloatObject.h>
#include <donut/object/NativeClosureObject.h>
#include <donut/object/DonutClosureObject.h>


namespace donut {

const static std::string TAG("Machine");

Context::Context(Handler<Clock> const& clk)
:time_(clk->now())
,interrupt_()
,stack_()
,callStack_()
{

}

Context::Context(Handler<Clock> const& clk, Context const& other)
:time_(clk->now())
,interrupt_(other.interrupt_)
,stack_(other.stack_)
,callStack_(other.callStack_)
{

}


Machine::Machine(Logger& log, Handler<Clock> const& clock, Handler<Heap> const& heap)
:log_(log)
,clock_(clock)
,heap_(heap)
,running_(false)
{
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

int Machine::findRevisionIndex(timestamp_t const& t) const
{
	for(int i=this->contextRevs_.size()-1; i>=0;--i){
		Context const& c = this->contextRevs_[i];
		if(t >= c.time_){
			return i;
			break;
		}
	}
	return -1;
}

Handler<Object> Machine::start( Handler<Source> const& src )
{
	if( this->isInterrupted() ){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is interrupted now. Call #resume instead.");
	}else if( !this->isCallstackEmpty() ) {
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is already running source now. Call #restart instead.");
	}
	this->clock_->tick();
	if( this->contextRevs_.empty() ){
		this->contextRevs_.push_back( Context( this->clock_ ) );
	}else{
		this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	}
	Handler<DonutClosureObject> entryPoint( heap_->createDonutClosureObject(src, src->getEntrypointID(), heap_->global()) );
	this->enterClosure(heap_->createNull(), entryPoint, std::vector<Handler<Object> >());
	if(this->log().d()){
		this->log().d(TAG, "Entrypoint context revision created.");
	}
	this->clock_->tick();
	this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	if(this->log().d()){
		this->log().d(TAG, "Start running...");
	}
	return this->run();
}

Handler<Object> Machine::restart()
{
	if( this->isInterrupted() ){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is interrupted now. Call #resume instead.");
	}else if( this->isCallstackEmpty() ) {
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is not running any source. Call #start with source first.");
	}
	this->clock_->tick();
	this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	if(this->log().d()){
		this->log().d(TAG, "Restart running...");
	}
	return this->run();
}

Handler<Object> Machine::resume(Handler<Object> const& obj)
{
	if( this->isCallstackEmpty() ) {
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is not running any source. Call #start with source first.");
	} else if( !this->isInterrupted() ) {
		DONUT_EXCEPTION(Exception, "[BUG] Oops. This machine is not interrupted now. Call #restart instead.");
	}
	this->clock_->tick();
	this->contextRevs_.push_back( Context( this->clock_, this->contextRevs_.back() ) );
	this->releaseInterrupt();
	this->pushStack(obj);
	if(this->log().d()){
		this->log().d(TAG, "Resuming...");
	}
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
	if( time >= last.time_ ){
		return bool(last.interrupt_);
	}
	// シークされてるので、インデックスを探す
	int const idx = this->findRevisionIndex(time);
	if( idx < 0 ){
		return false;
	}
	Context const& ctx = this->contextRevs_[idx];
	return bool(ctx.interrupt_);
}

bool Machine::isCallstackEmpty() const noexcept
{
	if( this->contextRevs_.empty() ) {
		return true;
	}
	unsigned int const time = this->clock_->now();
	// シークされてない
	Context const& last = this->contextRevs_.back();
	if( time >= last.time_ ){
		return last.callStack_.empty();
	}
	// シークされてるので、インデックスを探す
	int const idx = this->findRevisionIndex(time);
	if( idx < 0 ){
		return true;
	}
	Context const& ctx = this->contextRevs_[idx];
	return ctx.callStack_.empty();
}

void Machine::interrupt(Handler<Object> const& obj)
{
	Context& ctx = this->contextRevs_.back();
	Handler<Object>& i = ctx.interrupt_;
	if(i) {
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Already interrupted!!", i->repr(heap_).c_str());
	}
	i = obj;
}

void Machine::releaseInterrupt()
{
	Context& ctx = this->contextRevs_.back();
	ctx.interrupt_.reset();
}

Handler<Object> const& Machine::interrupt() const noexcept
{
	Context const& ctx = this->contextRevs_.back();
	return ctx.interrupt_;
}
bool Machine::isInterruptedNow() const noexcept
{
	Context const& ctx = this->contextRevs_.back();
	return bool(ctx.interrupt_);
}

void Machine::enterClosure(Handler<Object> const& self, Handler<DonutClosureObject> const& clos, std::vector<Handler<Object> > const& args)
{
	Handler<Closure> const& c = clos->closureCode();
	const std::size_t argsize = c->arglist().size();
	if(argsize != args.size()) {
		DONUT_EXCEPTION(Exception, "[BUG] Argument size does not match actual: %d != applied: %d", argsize, args.size());
	}
	{
		Handler<DonutObject> scope ( heap_->createEmptyDonutObject() );
		scope->set(heap_, "__scope__", clos);
		int cnt=0;
		for(Handler<Object> const& arg : args) {
			std::string const& argname = c->arglist().at(cnt++);
			scope->set(heap_, argname, arg);
		}
		this->callStack().push_back( Callchain(0, this->stack().size(), self, clos, scope) );
	}
}

bool Machine::leaveClosure()
{
	this->callStack().pop_back();
	return !this->callStack().empty(); //まだ実行するコールスタックが存在する
}

Handler<Object> Machine::run()
{
	RunLock lock(this);

	std::vector<Handler<Object> > arg;
	bool running = true;
	Instruction inst = 0;

	Instruction opcode = 0;
	Instruction constKind = 0;
	int constIndex = 0;
	while( running ){
		if(!this->fetchPC( inst )){ //このクロージャの終端に来ました
			running &= this->leaveClosure();
			continue;
		}
		Source::disasm(inst, opcode, constKind, constIndex);
		if(this->log().t()){
			this->log().t(TAG, format("$%04x ", this->pc())+this->src()->disasm(inst));
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
				this->pushStack( this->heap_->createDonutClosureObject(this->src(), constIndex, this->local()) );
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
				DONUT_EXCEPTION(Exception, "Unknwon const type: %d", constKind);
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
			Handler<Object> const nameObj = this->popStack();
			std::string const name = nameObj->toString(heap_);

			Handler<Object> obj = this->local();
			bool found = false;
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
				DONUT_EXCEPTION(Exception, "Unknwon var: %s", name.c_str());
			}
			break;
		}
		case Inst::DefVar: {
			Handler<Object> nameObj = this->popStack();
			std::string const name = nameObj->toString(heap_);
			Handler<Object> varObj = this->topStack();

			this->local()->set(heap_, name, varObj);
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
			arg.clear();
			for(unsigned int i=constIndex;i;--i){
				arg.push_back(this->popStack());
			}
			std::reverse(arg.begin(), arg.end());

			Handler<Object> closureObj = this->popStack();
			Handler<Object> destObj = this->popStack();

			if( Handler<NativeClosureObject> nclos = closureObj->tryCastToNativeClosureObject() ){
				this->pushStack( nclos->apply(heap_, destObj, arg) );
				if( unlikely(clock_->invokeMahcineRequest()) ){ //ネイティブ処理の実行後は時計操作が行われる可能性がある
					running &= !this->isInterruptedNow();
				}
			}else if( Handler<DonutClosureObject> dclos = closureObj->tryCastToDonutClosureObject() ){
				this->enterClosure(destObj, dclos, arg);
			}else{
				DONUT_EXCEPTION(Exception, "[BUG] Oops. \"%s\" is not callable.", closureObj->repr(heap_).c_str());
			}
			break;
		}
		case Inst::ConstructArray: {
			Handler<Object> obj(heap_->createEmptyDonutObject() );
			for(unsigned int i=constIndex;i>0;--i){
				Handler<Object> val = this->popStack();

				obj->set(heap_, toString(i-1), val);
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
			this->interrupt( this->popStack() );
			running = false;
			break;
		}
		case Inst::Return: {
			this->stack().resize( this->stackBase()+1 ); //戻り値の分の+1
			running &= this->leaveClosure();
			break;
		}
		default:
			DONUT_EXCEPTION(Exception,
					"[BUG] Oops. Unknwon opcode: closure<%s>@%08x=%08x => (%02x,%02x,%04x)",
					closureObject()->repr(heap_).c_str(), this->pc()-1, inst,opcode,constKind,constIndex);
		}
	}
	if(this->isInterruptedNow()) {
		return this->interrupt();
	}else{
		Handler<Object> result(this->popStack());
		if( !this->stack().empty() ){
			DONUT_EXCEPTION(Exception, "[BUG] Oops. Execution ended, but stack id not empty:%d", this->stack().size());
		}
		return result;
	}
}

}

