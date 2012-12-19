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

#include "HomuraObject.h"
#include "../../util/StringUtil.h"
#include "Heap.h"

namespace chisa {
namespace donut {

const std::string TAG("HomuraObject");

HomuraObject::HomuraObject(std::string const& providerName)
:NativeObject(providerName)
{
}

std::string HomuraObject::reprImpl(Handler<Heap> const& heap) const
{
	return util::format("Homura: %p", this);
}

void HomuraObject::onBackNotifyImpl(Handler<Heap> const& heap)
{

}
void HomuraObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{

}

void HomuraObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{

}

void HomuraObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{

}

void HomuraObject::bootstrap( Handler<Heap> const& heap )
{
	this->NativeObject::bootstrap(heap);
}


bool HomuraObject::hasImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return this->slot_.find(name) != this->slot_.end() ? true : this->NativeObject::hasImpl(heap, name);
}
bool HomuraObject::hasOwnImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return this->slot_.find(name) != this->slot_.end();
}
Handler<Object> HomuraObject::setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> obj)
{
	this->slot_.update(name, obj.get());
	return obj;
}
Handler<Object> HomuraObject::getImpl(Handler<Heap> const& heap, std::string const& name) const
{
	auto it = this->slot_.find(name);
	Object* const& p = (*it).second;
	return (it != this->slot_.end()) ? Handler<Object>::__internal__fromRawPointerWithoutCheck(p) : this->NativeObject::getImpl(heap, name);
}
void HomuraObject::markImpl(Handler<Heap> const& heap, int color)
{
	NativeObject::markImpl(heap, color);
	for(std::pair<std::string, Object*>& p : this->slot_){
		Object* const& pt = p.second;
		pt->mark(heap, color);
	}
}


/**********************************************************************************
 * save/load
 **********************************************************************************/

util::XValue HomuraObject::saveImpl( Handler<Heap> const& heap )
{
	using namespace util;
	Handler<XObject> xobj(new XObject);
	{
		Handler<XObject> xslot(new XObject);
		for(std::pair<std::string, Object*>& p : this->slot_){
			xslot->set(p.first, p.second->toDescriptor());
		}
		xobj->set("slot", xslot);
	}
	return xobj;
}
void HomuraObject::loadImpl( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace util;
	Handler<XObject> xobj(data.as<XObject>());
	for(std::pair<std::string, XValue> const& v : *(xobj->get<XObject>("slot"))){
		slot_.insert(v.first, heap->decodeDescriptor(v.second.as<object_desc_t>()).get());
	}
}

}}

