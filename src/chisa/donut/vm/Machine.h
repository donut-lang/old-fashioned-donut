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
#include "../object/World.h"
#include "../code/Code.h"

namespace chisa {
namespace donut {
class World;
typedef unsigned int pc_t;

struct Callchain {
	pc_t pc_;
	Handler<Object> self_;
	Handler<DonutClosureObject> closure_;
	Handler<DonutObject> context_;
public:
	Callchain(pc_t pc, Handler<Object> self, Handler<DonutClosureObject> closure, Handler<DonutObject> context)
	:pc_(pc), self_(self), closure_(closure), context_(context){
	}
};

class Machine {
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	World* const world_;
	pc_t pc_;
	Handler<Object> self_;
	Handler<DonutClosureObject> closure_;
	Handler<DonutObject> context_;
	std::vector<Instruction> const* asmlist_;
	std::vector<Handler<Object> > stack_;
	std::vector<Handler<Object> > local_;
	std::vector<Callchain> callStack_;
public:
	Machine(logging::Logger& log, World* world);
	virtual ~Machine() noexcept = default;
public:
	Handler<Object> start( const std::size_t closureIndex );
private:
	void enterClosure(const Handler<Object>& self, const Handler<DonutClosureObject>& clos, const Handler<Object>& args);
	bool returnClosure();
	Handler<DonutClosureObject> createClosure(const Handler<Closure>& closureCode);
private:
	Handler<Object> run();
};

}}
