/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/object/Object.h>
namespace donut {

HeapObject::HeapObject(HeapProvider* const provider)
:provider_(provider)
,id_(0)
,erased_(0)
,color_(0)
,useLocked_(false)
{
}

std::string HeapObject::providerNameImpl(Handler<Heap> const& heap) const
{
	return this->provider_->name();
}

}
