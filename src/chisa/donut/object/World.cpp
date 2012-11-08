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

#include "World.h"

namespace chisa {
namespace donut {

World::World(logging::Logger& log, Handler<Code> code)
:log_(log)
,code_(code)
,generation_(0)
,boolProxy_(this)
,intProxy_(this)
,nullProxy_(this)
{
	// TODO Auto-generated constructor stub

}

unsigned int World::nextGeneration()
{
	return ++this->generation_;
}

}}
