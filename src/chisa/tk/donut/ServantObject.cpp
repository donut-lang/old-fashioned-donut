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
#include "ServantObject.h"
#include "ElementObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("ServantObject");

ServantProvider::ServantProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
}

Handler<Heaven> ServantProvider::heaven() const
{
	return heaven_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ServantObject::ServantObject(ServantProvider* provider)
:ReactiveNativeObject(provider)
{
}

void ServantObject::bootstrap(const Handler<Heap>& heap, Handler<Servant> const& servant)
{
	this->ReactiveNativeObject::bootstrap(heap);
	this->servant_ = servant;
}

Handler<World> ServantObject::world() const
{
	return servant_->world();
}

Handler<Servant> ServantObject::servant() const
{
	return this->servant_;
}

std::string ServantObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(ServantObject: %p)", this);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

HaloServantProvider::HaloServantProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:ServantProvider(heap, "HaloServant", heaven)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

HaloServantObject::HaloServantObject(ServantProvider* provider)
:ServantObject(provider)
{
}

void HaloServantObject::bootstrap(const Handler<Heap>& heap, const Handler<HaloServant>& Servant)
{
}

Handler<World> HaloServantObject::world() const
{
}

Handler<HaloServant> HaloServantObject::servant() const
{
	return Handler<HaloServant>::__internal__fromRawPointerWithoutCheck(static_cast<HaloServant*>(ServantObject::servant().get()));
}

void HaloServantObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void HaloServantObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

HaloServantObject::ResultType HaloServantObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

HaloServantObject::ResultType HaloServantObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

XValue HaloServantObject::saveImpl(const Handler<Heap>& heap)
{
}

void HaloServantObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ElementServantProvider::ElementServantProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:ServantProvider(heap, "ElementServant", heaven)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ElementServantObject::ElementServantObject(ServantProvider* provider)
:ServantObject(provider)
{
}

void ElementServantObject::bootstrap(const Handler<Heap>& heap, const Handler<ElementServant>& Servant)
{
}

Handler<World> ElementServantObject::world() const
{
}

Handler<ElementServant> ElementServantObject::servant() const
{
	return Handler<ElementServant>::__internal__fromRawPointerWithoutCheck(static_cast<ElementServant*>(ServantObject::servant().get()));
}

void ElementServantObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void ElementServantObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

ElementServantObject::ResultType ElementServantObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

ElementServantObject::ResultType ElementServantObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

XValue ElementServantObject::saveImpl(const Handler<Heap>& heap)
{
}

void ElementServantObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

}}


