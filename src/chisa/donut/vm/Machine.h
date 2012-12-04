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

#pragma once
#include <vector>
#include "../object/Object.h"
#include "../object/Heap.h"
#include "../source/Source.h"

namespace chisa {
namespace donut {
class Heap;
typedef unsigned int pc_t;

struct Callchain {
	pc_t pc_;
	Handler<Object> self_;
	Handler<DonutClosureObject> closure_;
	Handler<DonutObject> scope_;
public:
	Callchain(pc_t pc, const Handler<Object>& self, const Handler<DonutClosureObject>& closure, const Handler<DonutObject>& scope)
	:pc_(pc), self_(self), closure_(closure), scope_(scope){
	}
};

struct Context {
	unsigned int time_;
	std::vector<Handler<Object> > stack_;
	std::vector<Handler<Object> > local_;
	std::vector<Callchain> callStack_;
	Context(const Handler<Clock>& clk);
	Context(const Handler<Clock>& clk, const Context& other);
	Context() = delete;
	~Context() noexcept = default;
};

class Machine : public HandlerBody<Machine> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private: //何があっても不変なもの。
	const Handler<Clock>& clock_;
	Handler<Heap> const heap_;
private: //時とともに変わっていくもの
	std::vector<Context> contextRevs_;
private: //それへのアクセス手段の提供。
	Handler<Object> const& self();
	Handler<DonutObject> const& scope();
	Handler<DonutClosureObject> const& closureObject();
	Handler<Source> const& src();
	Handler<Closure> const& closure();
	std::vector<Handler<Object> >& local();
	std::vector<Handler<Object> >& stack();
	std::vector<Callchain>& callStack();
	pc_t& pc();
	bool fetchPC( Instruction& inst );
public:
	Machine(logging::Logger& log, const Handler<Clock>& clock, const Handler<Heap>& heap);
	virtual ~Machine() noexcept = default;
	bool onFree() noexcept { return false; };
	void discardFuture();
	void discardHistory();
public:
	Handler<Object> start( const Handler<Source>& src );
	Handler<Object> startContinue( const Handler<Object>& obj );
	bool isInterrupted() const noexcept;
public:
	void seek( timestamp_t time );
private:
	Handler<Object> pushStack( const Handler<Object>& obj );
	Handler<Object> popStack();
	Handler<Object> topStack();
	void enterClosure(const Handler<Object>& self, const Handler<DonutClosureObject>& clos, const Handler<Object>& args);
	Handler<Object> run();
};

}}
