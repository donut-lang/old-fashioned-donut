/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <cinamo/Handler.h>
#include <donut/object/FloatObject.hpp>

namespace donut {
using namespace cinamo;

const std::string TAG("FloatObject");

FloatObject::FloatObject(HeapProvider* const provider)
:NativeObject(provider)
,value_(NAN)
{
}

std::string FloatObject::reprImpl(Handler<Heap> const& heap) const
{
	return std::string("float: ")+::cinamo::toString(value_);
}

std::string FloatObject::printImpl(Handler<Heap> const& heap) const
{
	return ::cinamo::toString(value_);
}

Handler<const FloatObject> FloatObject::toFloatObjectImpl() const
{
	return Handler<const FloatObject>::__internal__fromRawPointerWithoutCheck(this);
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

XValue FloatObject::saveImpl( Handler<Heap> const& heap )
{
	return XValue( this->value_ );
}
void FloatObject::loadImpl( Handler<Heap> const& heap, XValue const& data )
{
	const_cast<float&>(this->value_) = data.as<XFloat>();
}

}
