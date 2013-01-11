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
#include "AngelObject.h"
#include "ElementObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("AngelObject");

AngelProvider::AngelProvider(const Handler<Heap>& heap, const Handler<World>& world)
:Super(heap, "chisa::tk::AngelObject")
,world_(world)
{
}

Handler<World> AngelProvider::world() const
{
	return world_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelObject::AngelObject(AngelProvider* provider)
:Super(provider)
,world_(provider->world())
{
}

void AngelObject::bootstrap(const Handler<Heap>& heap)
{
	this->ReactiveNativeObject::bootstrap(heap);
}

std::string AngelObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(AngelObject: %p)", this);
}

Handler<World> AngelObject::world() const
{
	return this->world_.lock();
}

AngelObject::ResultType AngelObject::onBack(const Handler<Heap>& heap, AngelSnap const& val)
{
	//XXX
	return AngelObject::ResultType(true, AngelSnap());
}

AngelObject::ResultType AngelObject::onForward(const Handler<Heap>& heap, AngelSnap const& val)
{
	//XXX
	return AngelObject::ResultType(true, AngelSnap());
}

XValue AngelObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void AngelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void AngelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

void AngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

}}
