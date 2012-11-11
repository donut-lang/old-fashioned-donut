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

}

unsigned int World::nextGeneration()
{
	return ++this->generation_;
}

Handler<Object> World::createInt(const int& val)
{
	return Handler<Object>(IntProxy::toPointer(val));
}
Handler<Object> World::createBool(const bool& val)
{
	return Handler<Object>(BoolProxy::toPointer(val));
}
Handler<Object> World::createNull()
{
	return Handler<Object>(NullProxy::toPointer());
}

}}
