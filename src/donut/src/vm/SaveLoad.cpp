/**
 * Donut
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

#include <algorithm>
#include <donut/vm/Machine.h>


namespace donut {

const static std::string TAG("Machine");

/**********************************************************************************
 * save/load
 **********************************************************************************/
void Machine::bootstrap()
{
	this->contextRevs_.clear();
}

XValue Callchain::save()
{
	Handler<XObject> top(new XObject);
	top->set("pc", this->pc_);
	top->set("stack_base", this->stackBase_);
	top->set("self", this->self_->toDescriptor());
	top->set("closure", this->closure_->toDescriptor());
	top->set("scope", this->local_->toDescriptor());
	return top;
}

Callchain::Callchain(Handler<Heap> const& heap, XValue const& data)
{
	Handler<XObject> obj( data.as<XObject>() );
	this->pc_ = obj->get<decltype(this->pc_)>("pc");
	this->stackBase_ = obj->get<decltype(this->stackBase_)>("stack_base");
	this->self_ = heap->decodeDescriptor( obj->get<object_desc_t>("self") );
	this->closure_ = heap->decodeDescriptor( obj->get<object_desc_t>("closure") ).cast<DonutClosureObject>();
	this->local_ = heap->decodeDescriptor( obj->get<object_desc_t>("scope") ).cast<DonutObject>();
}

XValue Context::save()
{
	Handler<XObject> top(new XObject);
	// time
	top->set("time", this->time_);
	//interrupt
	top->set("interrupt", this->interrupt_->toDescriptor());
	{ // callstack
		Handler<XArray> list(new XArray);
		for( Callchain& chain : this->callStack_ ) {
			list->append(chain.save());
		}
		top->set("callstack", list);
	}
	{ //stack
		Handler<XArray> list(new XArray);
		for( Handler<Object> const& obj : this->stack_ ) {
			list->append( obj->toDescriptor() );
		}
		top->set("stack", list);
	}
	return top;
}
Context::Context(Handler<Heap> const& heap, XValue const& data)
{
	Handler<XObject> obj ( data.as<XObject>() );
	{ //time
		this->time_ = obj->get<unsigned int>("time");
	}
	{ //interrupt
		this->interrupt_ = heap->decodeDescriptor(obj->get<object_desc_t>("interrupt"));
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

XValue Machine::save()
{
	Handler<XArray> top(new XArray);
	//context_revsさえ保存すればよい
	for( Context& ctx : this->contextRevs_ ) {
		top->append( ctx.save() );
	}
	return top;
}
void Machine::load( XValue const& obj)
{
	for(auto data : *(obj.as<XArray>())){
		this->contextRevs_.push_back( Context(heap_, data) );
	}
	if( unlikely(!std::is_sorted(this->contextRevs_.begin(), this->contextRevs_.end(), Context::CompareByTime())) ){
		DONUT_EXCEPTION(Exception, "[BUG] Invalid save data. Revision data is not sorted.");
	}
}

}

