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

namespace chisa {
namespace donut {

Machine::Machine(ObjectPool& pool)
:pc_(0),pool_(pool)
{
	// TODO Auto-generated constructor stub

}

void Machine::start(Handler<Closure> clos)
{

}

void Machine::run()
{
	while(1){
		Handler<Object> clos(this->callStack_.back());
	}
}

}}
