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

#include "NativeObject.h"
#include <algorithm>

namespace chisa {
namespace donut {

const std::string TAG("ReactiveNativeObject");

std::string ReactiveNativeObject::toStringImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to String implicitly. Use toString()");
}

int ReactiveNativeObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Integer implicitly. Use toInteger()");
}

float ReactiveNativeObject::toFloatImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Float implicitly. Use toFloat()");
}

bool ReactiveNativeObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Bool implicitly. Use toBoolean()");
}

void ReactiveNativeObject::onSeekNotifyImpl(const Handler<Heap>& heap)
{

}

void ReactiveNativeObject::onDiscardHistoryNotifyImpl(const Handler<Heap>& heap)
{

}

void ReactiveNativeObject::onDiscardFutureNotifyImpl(const Handler<Heap>& heap)
{

}

void ReactiveNativeObject::bootstrap( Handler<Heap> const& heap )
{
	this->NativeObject::bootstrap(heap);
}


/**********************************************************************************
 * save/load
 **********************************************************************************/

util::XValue ReactiveNativeObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> top(new XObject);
	top->set("base", this->NativeObject::save(heap));
	return top;
}
void ReactiveNativeObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> top(data.as<XObject>());
	this->NativeObject::load(heap, top->get<XValue>("base"));
}

}}

