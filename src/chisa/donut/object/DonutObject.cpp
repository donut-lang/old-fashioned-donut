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

DonutObject::DonutObject(const std::string& providerName)
:HeapObject(providerName)
{

}


std::string DonutObject::toStringImpl(const Handler<Heap>& heap) const
{
	return util::format("(DonutObject %p)", this);
}

int DonutObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert object to int.");
}

float DonutObject::toFloatImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to float.");
}

bool DonutObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to bool.");
}

bool DonutObject::haveImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return haveOwnImpl(heap, name) || (haveOwnImpl(heap, "__proto__") && getImpl(heap, "__proto__")->have(heap, name));
}

bool DonutObject::haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const
{
	auto it = this->slots_.find(name);
	return it != this->slots_.end() && it->second.have();
}

Handler<Object> DonutObject::setImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj)
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

Handler<Object> DonutObject::getImpl(const Handler<Heap>& heap, const std::string& name) const
{
	auto it = this->slots_.find(name);
	if(it != this->slots_.end()){
		return Handler<Object>::__internal__fromRawPointerWithoutCheck(it->second.load());
	}
	if(this->haveOwnImpl(heap,"__proto__")){
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
void DonutObject::onSeekNotifyImpl(const Handler<Heap>& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onSeekNotify(heap);
	}
}
void DonutObject::onDiscardFutureNotifyImpl(const Handler<Heap>& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onDiscardFutureNotify( heap );
	}
}

void DonutObject::onDiscardHistoryNotifyImpl(const Handler<Heap>& heap)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.onDiscardHistoryNotify( heap );
	}
}

}}
