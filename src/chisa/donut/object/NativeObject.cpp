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
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "Heap.h"
namespace chisa {
namespace donut {

NativeObject::NativeObject(Heap* const heap, const std::string& providerName)
:HeapObject(heap, providerName)
{
	this->prototype_ = heap->getProvider(providerName)->prototype().get();
}

std::string NativeObject::toStringImpl() const
{
	return util::format("(NativeObject %p)", this);
}

int NativeObject::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to int.");
}

float NativeObject::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to float.");
}

bool NativeObject::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to bool.");
}

bool NativeObject::haveImpl(const std::string& name) const
{
	return this->prototype_->have(heap(), name);
}

bool NativeObject::haveOwnImpl(const std::string& name) const
{
	return false;
}

Handler<Object> NativeObject::storeImpl(const std::string& name, Handler<Object> obj)
{
	return obj;
}

Handler<Object> NativeObject::loadImpl(const std::string& name) const
{
	return this->prototype_->load(heap(), name);
}

void NativeObject::seekImpl(timestamp_t time)
{

}

}}


