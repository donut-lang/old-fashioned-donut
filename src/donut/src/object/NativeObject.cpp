/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/object/NativeObject.h>

namespace donut {

NativeObject::NativeObject(HeapProvider* const provider)
:HeapObject(provider)
,prototype_(nullptr)
{
}

std::string NativeObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("(NativeObject for \"%s\" %p)", this->provider()->name().c_str(), this);
}

bool NativeObject::hasImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return this->prototype_->has(heap, name);
}

bool NativeObject::hasOwnImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return false;
}

Handler<Object> NativeObject::setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> const& obj)
{
	return obj;
}

Handler<Object> NativeObject::getImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return this->prototype_->get(heap, name);
}

void NativeObject::bootstrap(Handler<Heap> const& heap) {
	this->prototype_ = this->provider()->prototype().get();
}

XValue NativeObject::save( Handler<Heap> const& heap )
{
	Handler<XObject> obj (new XObject());
	obj->set("prototype", this->prototype_->toDescriptor());
	obj->set("content",  this->saveImpl(heap));
	return obj;
}
void NativeObject::load( Handler<Heap> const& heap, XValue const& data )
{
	Handler<XObject> obj ( data.as<XObject>() );
	this->prototype_ = heap->decodeHeapDescriptor( obj->get<object_desc_t>("prototype") ).cast<DonutObject>().get();
	this->loadImpl( heap, obj->get<XValue>("content") );
}

Handler<NativeClosureObject> NativeClosureObject::tryCastToNativeClosureObjectImpl()
{
	return Handler<NativeClosureObject>::__internal__fromRawPointerWithoutCheck(this);
}

}
