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

#include "NativeObject.h"
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "Heap.h"

namespace chisa {
namespace donut {
static const std::string TAG("NativeClosureObject");

std::string NativeClosureObject::toStringImpl(const Handler<Heap>& heap) const
{
	return util::format("(NativeClosureObject %p)", this);
}

int NativeClosureObject::toIntImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert NativeClosureObject to int.");
}

float NativeClosureObject::toFloatImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert NativeClosureObject to float.");
}

bool NativeClosureObject::toBoolImpl(const Handler<Heap>& heap) const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert NativeClosureObject to bool.");
}

bool NativeClosureObject::haveImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return false;
}

bool NativeClosureObject::haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const
{
	return false;
}

Handler<Object> NativeClosureObject::setImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj)
{
	heap->log().w(TAG, "NativeClosureObject cannot have any property.");
	return obj;
}

Handler<Object> NativeClosureObject::getImpl(const Handler<Heap>& heap, const std::string& name) const
{
	throw DonutException(__FILE__, __LINE__, "Native Closure does not have any properety.");
}

void NativeClosureObject::onSeekNotifyImpl(const Handler<Heap>& heap)
{

}
void NativeClosureObject::onDiscardHistoryNotifyImpl(const Handler<Heap>& heap)
{

}

void NativeClosureObject::onDiscardFutureNotifyImpl(const Handler<Heap>& heap)
{

}

void NativeClosureObject::bootstrap( std::string const& name )
{
	const_cast<std::string&>( this->closureName_ ) = name;
}

util::XValue NativeClosureObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> obj(new XObject);
	obj->set("name", this->closureName_);
	obj->set("content", this->saveImpl(heap));
	return obj;
}

void NativeClosureObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> obj(data.as<XObject>());
	const_cast<std::string&>( this->closureName_ ) = obj->get<XString>("name");
	this->loadImpl(heap, obj->get<XValue>("content"));
}

//-----------------------------------------------------------------------------

void PureNativeClosureObject::bootstrap( std::string const& name, PureNativeClosureObject::Signature f )
{
	this->NativeClosureObject::bootstrap(name);
	this->func_ = f;
}


util::XValue PureNativeClosureObject::saveImpl( Handler<Heap> const& heap )
{
	return util::XValue();
}

void PureNativeClosureObject::loadImpl( Handler<Heap> const& heap, util::XValue const& data )
{
	this->func_ = heap->getProvider(this->closureName())->findClosureEntry("name");
}

}}


