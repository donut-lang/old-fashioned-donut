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

#include <sstream>
#include "Heap.h"
#include "Object.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("DonutObject");

DonutObject::DonutObject(std::string const& providerName)
:HeapObject(providerName)
{

}


std::string DonutObject::toStringImpl(Handler<Heap> const& heap) const
{
	return util::format("(DonutObject %p)", this);
}

int DonutObject::toIntImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert object to int.");
}

float DonutObject::toFloatImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to float.");
}

bool DonutObject::toBoolImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to bool.");
}

bool DonutObject::hasImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return hasOwnImpl(heap, name) || (hasOwnImpl(heap, "__proto__") && getImpl(heap, "__proto__")->has(heap, name));
}

bool DonutObject::hasOwnImpl(Handler<Heap> const& heap, std::string const& name) const
{
	auto it = this->slots_.find(name);
	return it != this->slots_.end() && it->second.have();
}

Handler<Object> DonutObject::setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> obj)
{
	auto it = this->slots_.find(name);
	if(it == this->slots_.end()){
		std::pair<std::string, Slot> p(name, Slot());
		p.second.store(heap, obj.get());
		this->slots_.insert( p );
	}else{
		it->second.store(heap, obj.get());
	}
	return obj;
}

Handler<Object> DonutObject::getImpl(Handler<Heap> const& heap, std::string const& name) const
{
	auto it = this->slots_.find(name);
	if(it != this->slots_.end()){
		return Handler<Object>::__internal__fromRawPointerWithoutCheck(it->second.load());
	}
	if(this->hasOwnImpl(heap,"__proto__")){
		return getImpl(heap,"__proto__")->get(heap, name);
	}
	{
		std::stringstream ss;
		ss << "\"" << name << "\" not found." << std::endl;
		ss << "available: " << std::endl;
		for(auto i : this->slots_){
			ss << "  -> " << i.first << std::endl;
		}
		throw DonutException(__FILE__, __LINE__, ss.str());
	}
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

util::XValue DonutObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> obj(new XObject);
	{
		Handler<XObject> slots(new XObject);
		for( std::pair<std::string const, Slot>& p: this->slots_ ){
			slots->set(p.first, p.second.save());
		}
		obj->set("slot", slots);
	}
	obj->set("content", this->saveImpl(heap));
	return obj;
}

void DonutObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> obj(data.as<XObject>());
	{
		for(std::pair<std::string, XValue>& p : *(obj->get<XObject>("slot"))){
			this->slots_.insert( std::pair<std::string const, Slot>( p.first, Slot(heap, p.second) ));
		}
	}
	this->loadImpl( heap, obj->get<XValue>("content") );
}
/**********************************************************************************
 * from clock
 **********************************************************************************/
void DonutObject::onBackNotifyImpl(Handler<Heap> const& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onBackNotify(heap);
	}
}
void DonutObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onForwardNotify(heap);
	}
}

void DonutObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onDiscardFutureNotify( heap );
	}
}

void DonutObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onDiscardHistoryNotify( heap );
	}
}

}}
