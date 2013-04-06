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

#include <cinamo/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/source/Source.h>
#include <donut/object/DonutClosureObject.h>

namespace donut {
using namespace cinamo;

const std::string TAG("DonutClosureObject");

DonutClosureObject::DonutClosureObject(HeapProvider* const provider)
:DonutObject(provider)
,src_(nullptr)
,index_(-1)
,asm_(nullptr)
{
}

std::string DonutClosureObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("(DonutClosureObject %p)", this);
}

void DonutClosureObject::bootstrap(Handler<Heap> const& heap, Handler<Source> const& src, unsigned int const& closureIndex, Handler<Object> const& scope)
{
	const_cast<Handler<Source>&>(this->src_) = src;
	const_cast<unsigned int&>(this->index_) = closureIndex;
	const_cast<Handler<Closure>&>(this->asm_) = src->getClosure(closureIndex);
	if(scope){
		this->set(heap, "__scope__", scope);
	}
}

XValue DonutClosureObject::saveImpl( Handler<Heap> const& heap )
{
	Handler<XObject> top(new XObject);
	top->set("src", this->src_->id());
	top->set("index", this->index_);
	return top;
}
void DonutClosureObject::loadImpl(Handler<Heap> const& heap, XValue const& data)
{
	Handler<XObject> top(data.as<XObject>());
	const_cast<Handler<Source>&>(this->src_) = heap->decodeSourceID(top->get<int>("src"));
	const_cast<unsigned int&>(this->index_) = top->get<unsigned int>("index");
	const_cast<Handler<Closure>&>(this->asm_) = this->src_->getClosure(this->index_);
}

Handler<DonutClosureObject> DonutClosureObject::tryCastToDonutClosureObjectImpl()
{
	return Handler<DonutClosureObject>::__internal__fromRawPointerWithoutCheck(this);
}

}
