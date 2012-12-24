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

#include "StringObject.h"
#include <algorithm>

namespace donut {

const std::string TAG("StringObject");

StringObject::StringObject(HeapProvider* const provider)
:NativeObject(provider)
{

}

std::string StringObject::reprImpl(Handler<Heap> const& heap) const
{
	return std::string("string: ")+this->str_;
}

Handler<const StringObject> StringObject::toStringObjectImpl() const
{
	return Handler<const StringObject>::__internal__fromRawPointerWithoutCheck(this);
}

void StringObject::onBackNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::bootstrap( Handler<Heap> const& heap, std::string const& val )
{
	this->NativeObject::bootstrap(heap);
	const_cast<std::string&>(this->str_) = val;
}


/**********************************************************************************
 * save/load
 **********************************************************************************/

XValue StringObject::saveImpl( Handler<Heap> const& heap )
{
	return XValue(this->str_);
}
void StringObject::loadImpl( Handler<Heap> const& heap, XValue const& data )
{
	const_cast<std::string&>(this->str_) = data.as<XString>();
}

}

