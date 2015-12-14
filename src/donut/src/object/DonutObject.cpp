/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <sstream>
#include <cinamo/String.h>
#include <donut/Exception.hpp>
#include <donut/object/Heap.hpp>
#include <donut/object/DonutObject.hpp>

namespace donut {
using namespace cinamo;

static const std::string TAG("DonutObject");

DonutObject::DonutObject(HeapProvider* const provider)
:HeapObject(provider)
{

}

std::string DonutObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("(DonutObject %p)", this);
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

Handler<Object> DonutObject::setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> const& obj)
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
		DONUT_EXCEPTION(Exception, ss.str());
	}
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

void DonutObject::bootstrap()
{
	this->slots_.clear();
}
XValue DonutObject::save( Handler<Heap> const& heap )
{
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

void DonutObject::load( Handler<Heap> const& heap, XValue const& data )
{
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

}
