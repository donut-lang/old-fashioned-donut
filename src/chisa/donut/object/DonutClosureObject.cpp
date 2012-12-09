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

#include "Heap.h"
#include "Object.h"
#include "../Exception.h"

namespace chisa {
namespace donut {

const std::string TAG("DonutClosureObject");

void DonutClosureObject::bootstrap(const Handler<Heap>& heap, Handler<Source> const& src, unsigned int const& closureIndex, const Handler<Object>& scope)
{
	const_cast<Handler<Source>&>(this->src_) = src;
	const_cast<unsigned int&>(this->index_) = closureIndex;
	const_cast<Handler<Closure>&>(this->asm_) = src->getClosure(closureIndex);
	if(scope){
		this->set(heap, "__scope__", scope);
	}
}

}}
