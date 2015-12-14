/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include <cinamo/Handler.h>
#include <donut/object/StringObject.hpp>

namespace donut {
using namespace cinamo;

const std::string TAG("StringObject");

StringObject::StringObject(HeapProvider* const provider)
:NativeObject(provider)
{

}

std::string StringObject::reprImpl(Handler<Heap> const& heap) const
{
	return std::string("string: ")+this->str_;
}

std::string StringObject::printImpl(Handler<Heap> const& heap) const
{
	return this->str_;
}

Handler<const StringObject> StringObject::toStringObjectImpl() const
{
	return Handler<const StringObject>::__internal__fromRawPointerWithoutCheck(this);
}

void StringObject::onBackNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{

}

void StringObject::bootstrap( Handler<Heap> const& heap, std::string const& val )
{
	this->NativeObject::bootstrap(heap);
	const_cast<std::string&>(this->str_) = val;
}


/**********************************************************************************
 * save/load
 **********************************************************************************/

XValue StringObject::saveImpl( Handler<Heap> const& heap )
{
	return XValue(this->str_);
}
void StringObject::loadImpl( Handler<Heap> const& heap, XValue const& data )
{
	const_cast<std::string&>(this->str_) = data.as<XString>();
}

}
