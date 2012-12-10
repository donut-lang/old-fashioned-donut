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

NativeObject::NativeObject(const std::string& providerName)
:HeapObject(providerName)
,prototype_(nullptr)
{
}

std::string NativeObject::toStringImpl(const Handler<Heap>& heap) const
{
	return util::format("(NativeObject %p)", this);
}

int NativeObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to int.");
}

float NativeObject::toFloatImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to float.");
}

bool NativeObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to NativeObject to bool.");
}

bool NativeObject::hasImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return this->prototype_->has(heap, name);
}

bool NativeObject::hasOwnImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return false;
}

Handler<Object> NativeObject::setImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj)
{
	return obj;
}

Handler<Object> NativeObject::getImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return this->prototype_->get(heap, name);
}

void NativeObject::onSeekNotifyImpl(const Handler<Heap>& heap)
{

}

void NativeObject::bootstrap(Handler<Heap> const& heap) {
	this->prototype_ = heap->getHeapProvider(this->providerName())->prototype().get();
}

util::XValue NativeObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> obj (new XObject());
	obj->set("prototype", this->prototype_->toDescriptor());
	obj->set("content",  this->saveImpl(heap));
	return obj;
}
void NativeObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> obj ( data.as<XObject>() );
	this->prototype_ = heap->decodeHeapDescriptor( obj->get<object_desc_t>("prototype") ).cast<DonutObject>().get();
	this->loadImpl( heap, obj->get<XValue>("content") );
}


}}


