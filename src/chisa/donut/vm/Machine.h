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

class Machine {
	typedef unsigned int pc_t;
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	pc_t pc_;
	std::vector<Handler<Object> > stack_;
	std::vector<Handler<Object> > local_;
	std::vector<Handler<Object> > selfStack_;
	std::vector<std::pair<Handler<ClosureObject>, pc_t> > callStack_;
	Handler<ClosureObject> closure_;
	World* const world_;
public:
	Machine(logging::Logger& log, World* world);
	virtual ~Machine() noexcept = default;
public:
	void start( const std::size_t closureIndex );
private:
	void enterClosure(Handler<Closure> clos);
	void returnClosure();
private:
	void run();
};

}}
