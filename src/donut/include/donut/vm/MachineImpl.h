/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include "../object/Object.h"
#include "../object/Heap.h"
#include "../source/Source.h"

namespace donut {

inline Handler<Object> const& Machine::self()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.self_;
}

inline Handler<DonutObject> const& Machine::local()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.local_;
}

inline Handler<DonutClosureObject> const& Machine::closureObject()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.closure_;
}

inline Handler<Source> const& Machine::src()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	Handler<DonutClosureObject> const& clos = chain.closure_;
	return clos->source();
}

inline Handler<Closure> const& Machine::closure()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	Handler<DonutClosureObject> const& clos = chain.closure_;
	return clos->closureCode();
}

inline pc_t& Machine::pc()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.pc_;
}

inline std::vector<Callchain>& Machine::callStack()
{
	Context& ctx =  this->contextRevs_.back();
	return ctx.callStack_;
}

inline std::vector<Handler<Object> >& Machine::stack()
{
	Context& ctx =  this->contextRevs_.back();
	return ctx.stack_;
}

inline unsigned int Machine::stackBase()
{
	Context& ctx =  this->contextRevs_.back();
	Callchain& chain = ctx.callStack_.back();
	return chain.stackBase_;
}

inline void Machine::pushStack( Handler<Object> const& obj )
{
	this->stack().push_back( obj );
}
inline Handler<Object> Machine::popStack()
{
	std::vector<Handler<Object> >& stack = this->stack();
	Handler<Object> obj( stack.back() );
	stack.pop_back();
	return obj;
}
inline Handler<Object> Machine::topStack()
{
	return this->stack().back();
}

}
