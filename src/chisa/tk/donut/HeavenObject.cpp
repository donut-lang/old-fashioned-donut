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

#include <functional>
#include "HeavenObject.h"
#include "ElementObject.h"
#include "../Heaven.h"

namespace chisa {
namespace tk {

const static std::string TAG("HeavenObject");

HeavenProvider::HeavenProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "chisa::tk::HeavenObject")
{
	this->registerReactiveNativeClosure("invokeAngel", [](HeavenObject* obj)->ResultType{

		return std::tuple<Handler<Object>,bool,HeavenSideEffect>();
	});
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

HeavenObject::HeavenObject(HeavenProvider* provider)
:Super(provider)
{
}

void HeavenObject::bootstrap(const Handler<Heap>& heap, Handler<Heaven> const& heaven)
{
	this->Super::bootstrap(heap);
	this->heaven_ = heaven;
}

std::string HeavenObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(HeavenObject: %p)", this);
}

HeavenObject::ResultType HeavenObject::onBack(const Handler<Heap>& heap, const HeavenSideEffect& val)
{
	//XXX
	return ResultType(true, HeavenSideEffect());
}

HeavenObject::ResultType HeavenObject::onForward(const Handler<Heap>& heap, const HeavenSideEffect& val)
{
	//XXX
	return ResultType(true, HeavenSideEffect());
}

XValue HeavenObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void HeavenObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void HeavenObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

void HeavenObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

}}
