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

#include <iostream>
#include <tarte/String.h>
#include <tarte/XVal.h>
#include <donut/object/Heap.h>
#include <donut/object/StringObject.h>

#include "SystemObject.h"
#include "SystemProvider.h"

namespace donut_cli {
using namespace donut;
using namespace tarte;

const std::string TAG("SystemObject");

SystemObject::SystemObject(HeapProvider* const provider)
:ReactiveNativeObject(provider)
{
}

void SystemObject::bootstrap(Handler<Heap> const& heap)
{
	this->ReactiveNativeObject::bootstrap(heap);
	const_cast<std::vector<std::string>& >(this->args_) = dynamic_cast<SystemProvider*>(this->provider())->args();
}

Handler<Object> SystemObject::println(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)
{
	for(Handler<Object> const& obj : arg){
		std::cout << obj->print(heap);
	}
	std::cout << std::endl;
	return self;
}

Handler<Object> SystemObject::print(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)
{
	for(Handler<Object> const& obj : arg){
		std::cout << obj->print(heap);
	}
	return self;
}

Handler<Object> SystemObject::args(Handler<Heap> const& heap, Handler<Object> const& _self, std::vector<Handler<Object> > const& arg)
{
	Handler<SystemObject> self(_self.cast<SystemObject>());
	Handler<Object> obj(heap->createDonutObject());
	for(std::size_t i=0;i<self->args_.size();++i) {
		std::string const& v = self->args_[i];
		obj->set(heap, i, heap->createStringObject(v));
	}
	return obj;
}

std::string SystemObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("System: %p", this);
}

XValue SystemObject::onBack(Handler<Heap> const& heap, XValue const& val)
{
	return XValue("Do nothing");
}
XValue SystemObject::onForward(Handler<Heap> const& heap, XValue const& val)
{
	return XValue("Do nothing");
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

XValue SystemObject::saveImpl( Handler<Heap> const& heap )
{
	return XValue();
}
void SystemObject::loadImpl( Handler<Heap> const& heap, XValue const& data )
{
}

}

