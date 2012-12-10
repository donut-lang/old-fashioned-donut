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

const std::string TAG("FloatObject");

std::string FloatObject::toStringImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to String implicitly. Use toString()");
}

int FloatObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Integer implicitly. Use toInteger()");
}

float FloatObject::toFloatImpl(const Handler<Heap>& heap) const
{
	return this->value_;
}

bool FloatObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Float to Bool implicitly. Use toBoolean()");
}

void FloatObject::onSeekNotifyImpl(const Handler<Heap>& heap)
{

}

void FloatObject::onDiscardHistoryNotifyImpl(const Handler<Heap>& heap)
{

}

void FloatObject::onDiscardFutureNotifyImpl(const Handler<Heap>& heap)
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

