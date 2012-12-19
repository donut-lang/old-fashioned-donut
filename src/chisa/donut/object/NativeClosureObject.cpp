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

#include "NativeClosureObject.h"
#include "ReactiveNativeObject.h"
#include "StringObject.h"
#include "FloatObject.h"
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "Heap.h"

namespace chisa {
namespace donut {
static const std::string TAG("NativeClosureObject");

bool NativeClosureObject::hasImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return false;
}

bool NativeClosureObject::hasOwnImpl(Handler<Heap> const& heap, std::string const& name) const
{
	return false;
}

Handler<Object> NativeClosureObject::setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> obj)
{
	heap->log().w(TAG, "NativeClosureObject cannot have any property.");
	return obj;
}

Handler<Object> NativeClosureObject::getImpl(Handler<Heap> const& heap, std::string const& name) const
{
	throw DonutException(__FILE__, __LINE__, "Native Closure does not have any properety.");
}

void NativeClosureObject::onBackNotifyImpl(Handler<Heap> const& heap)
{

}
void NativeClosureObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{

}
void NativeClosureObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{

}

void NativeClosureObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{

}

void NativeClosureObject::bootstrap( std::string const& objectProviderName, std::string const& closureName )
{
	const_cast<std::string&>( this->objectProviderName_ ) = objectProviderName;
	const_cast<std::string&>( this->closureName_ ) = closureName;
}

util::XValue NativeClosureObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> obj(new XObject);
	obj->set("objectProviderName", this->objectProviderName_);
	obj->set("closureName", this->closureName_);
	obj->set("content", this->saveImpl(heap));
	return obj;
}

void NativeClosureObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> obj(data.as<XObject>());
	const_cast<std::string&>( this->objectProviderName_ ) = obj->get<XString>("objectProviderName");
	const_cast<std::string&>( this->closureName_ ) = obj->get<XString>("closureName");
	this->loadImpl(heap, obj->get<XValue>("content"));
}

/**********************************************************************************************************************
 * PureNativeClosureObject
 **********************************************************************************************************************/

void PureNativeClosureObject::bootstrap( std::string const& objectProviderName, std::string const& closureName, PureNativeClosureObject::Signature f )
{
	this->NativeClosureObject::bootstrap(objectProviderName, closureName);
	this->func_ = f;
}

std::string PureNativeClosureObject::reprImpl(Handler<Heap> const& heap) const
{
	return util::format("PureNativeClosure %s#%s at %p", this->objectProviderName().c_str(), this->closureName().c_str(), this);
}

Handler<Object> PureNativeClosureObject::apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const
{
	return func_(heap, self,arg);
}

util::XValue PureNativeClosureObject::saveImpl( Handler<Heap> const& heap )
{
	return util::XValue();
}

void PureNativeClosureObject::loadImpl( Handler<Heap> const& heap, util::XValue const& data )
{
	this->func_ = heap->findProvider(this->objectProviderName())->findPureNativeClosureEntry(this->closureName());
}

/**********************************************************************************************************************
 * ReactiveNativeClosureObject
 **********************************************************************************************************************/

void ReactiveNativeClosureObject::bootstrap( std::string const& objectProviderName, std::string const& closureName, ReactiveNativeClosureObject::Signature f )
{
	this->NativeClosureObject::bootstrap(objectProviderName, closureName);
	this->func_ = f;
}

std::string ReactiveNativeClosureObject::reprImpl(Handler<Heap> const& heap) const
{
	return util::format("ReactiveNativeClosure %s#%s at %p", this->objectProviderName().c_str(), this->closureName().c_str(), this);
}

Handler<Object> ReactiveNativeClosureObject::apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const
{
	Handler<ReactiveNativeObject> obj;
	if(!self->isObject() || !(obj = self.tryCast<ReactiveNativeObject>())){
		throw DonutException(__FILE__, __LINE__, "[BUG] ReactiveNativeClosure must be applied only to ReactiveNativeObject.");
	}
	Handler<Object> result;
	util::XValue value;
	std::tie(result, value) = this->func_(heap, obj, arg);
	if(value.is<util::XNull>()) {
		heap->clock()->discardHistory();
	}else{
		obj->registerReaction(heap->clock()->now(), value);
	}
	return result;
}
util::XValue ReactiveNativeClosureObject::saveImpl(Handler<Heap> const& heap)
{
	return util::XValue();
}
void ReactiveNativeClosureObject::loadImpl(Handler<Heap> const& heap, util::XValue const& data)
{
	this->func_ = heap->findProvider(this->objectProviderName())->findReactiveNativeClosureEntry(this->closureName());
}

}}


