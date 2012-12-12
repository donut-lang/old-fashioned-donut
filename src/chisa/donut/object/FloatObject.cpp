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
#include <algorithm>

namespace chisa {
namespace donut {

const std::string TAG("FloatObject");

FloatObject::FloatObject(std::string const& providerName)
:NativeObject(providerName)
,value_(NAN)
{
}

std::string FloatObject::toStringImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to String implicitly. Use toString()");
}

int FloatObject::toIntImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Integer implicitly. Use toInteger()");
}

float FloatObject::toFloatImpl(Handler<Heap> const& heap) const
{
	return this->value_;
}

bool FloatObject::toBoolImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Bool implicitly. Use toBoolean()");
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

