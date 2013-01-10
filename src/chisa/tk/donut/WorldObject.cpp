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
#include "WorldObject.h"
#include "ElementObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("WorldObject");

WorldProvider::WorldProvider(const Handler<Heap>& heap, const Handler<World>& world)
:Super(heap, "chisa::tk::WorldObject")
,world_(world)
{
	this->registerPureNativeClosure("getElementById",
			std::function<Handler<ElementObject>(WorldObject* wobj, std::string const& name)>(
					[&](WorldObject* wobj, std::string const& name)->Handler<ElementObject>{
				Handler<World> const world = wobj->world();
				return world->getElementById(name)->getElementObject();
			}));
}

Handler<World> WorldProvider::world() const
{
	return world_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

WorldObject::WorldObject(WorldProvider* provider)
:Super(provider)
,world_(provider->world())
{
}

void WorldObject::bootstrap(const Handler<Heap>& heap)
{
	this->ReactiveNativeObject::bootstrap(heap);
}

std::string WorldObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(WorldObject: %p)", this);
}

Handler<World> WorldObject::world() const
{
	return this->world_.lock();
}

WorldObject::ResultType WorldObject::onBack(const Handler<Heap>& heap, const XValue& val)
{
	//XXX
	return ResultType(true, XValue());
}

WorldObject::ResultType WorldObject::onForward(const Handler<Heap>& heap, const XValue& val)
{
	//XXX
	return ResultType(true, XValue());
}

XValue WorldObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void WorldObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

}}
