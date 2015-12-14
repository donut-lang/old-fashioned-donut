/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <iostream>
#include <cinamo/String.h>
#include <cinamo/XVal.h>
#include <donut/object/Heap.hpp>
#include <donut/object/StringObject.hpp>

#include "SystemObject.hpp"
#include "SystemProvider.hpp"

namespace donut_cli {
using namespace donut;
using namespace cinamo;

const std::string TAG("SystemObject");

SystemObject::SystemObject(HeapProvider* const provider)
:Super(provider)
{
}

void SystemObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void SystemObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

void SystemObject::bootstrap(Handler<Heap> const& heap)
{
	this->ReactiveNativeObject::bootstrap(heap);
	const_cast<std::vector<std::string>& >(this->args_) = dynamic_cast<SystemProvider*>(this->provider())->args();
}

Handler<Object> SystemObject::println(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)
{
	for(Handler<Object> const& obj : arg){
		std::cout << obj->print(heap);
	}
	std::cout << std::endl;
	return self;
}

Handler<Object> SystemObject::print(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)
{
	for(Handler<Object> const& obj : arg){
		std::cout << obj->print(heap);
	}
	return self;
}

Handler<Object> SystemObject::readline(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)
{
	std::string result;
	std::cin >> result;
	return Handler<Object>(heap->createStringObject(result));
}

Handler<Object> SystemObject::args(Handler<Heap> const& heap, Handler<Object> const& _self, std::vector<Handler<Object> > const& arg)
{
	Handler<SystemObject> self(_self.cast<SystemObject>());
	Handler<Object> obj(heap->createDonutObject());
	for(std::size_t i=0;i<self->args_.size();++i) {
		std::string const& v = self->args_[i];
		obj->set(heap, i, heap->createStringObject(v));
	}
	return obj;
}

std::string SystemObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("System: %p", this);
}

typename SystemObject::ResultType SystemObject::onBack(Handler<Heap> const& heap, XValue const& val)
{
	return Just<XValue>(XValue("Do nothing"));
}

typename SystemObject::ResultType SystemObject::onForward(Handler<Heap> const& heap, XValue const& val)
{
	return Just<XValue>(XValue("Do nothing"));
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

XValue SystemObject::saveImpl( Handler<Heap> const& heap )
{
	return XValue();
}
void SystemObject::loadImpl( Handler<Heap> const& heap, XValue const& data )
{
}

}
