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

#include "FloatObject.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

const std::string TAG("FloatObject");

FloatObject::FloatObject(HeapProvider* const provider)
:NativeObject(provider)
,value_(NAN)
{
}

std::string FloatObject::reprImpl(Handler<Heap> const& heap) const
{
	return std::string("float: ")+util::toString(value_);
}

Handler<const FloatObject> FloatObject::toFloatObjectImpl() const
{
	return Handler<const FloatObject>::__internal__fromRawPointerWithoutCheck(this);
}
void FloatObject::onBackNotifyImpl(Handler<Heap> const& heap)
{

}
void FloatObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{

}

void FloatObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{

}

void FloatObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{

}

void FloatObject::bootstrap( Handler<Heap> const& heap, float const& val )
{
	this->NativeObject::bootstrap(heap);
	const_cast<float&>(this->value_) = val;
}


/**********************************************************************************
 * save/load
 **********************************************************************************/

util::XValue FloatObject::saveImpl( Handler<Heap> const& heap )
{
	return util::XValue( this->value_ );
}
void FloatObject::loadImpl( Handler<Heap> const& heap, util::XValue const& data )
{
	const_cast<float&>(this->value_) = data.as<util::XFloat>();
}

}}

