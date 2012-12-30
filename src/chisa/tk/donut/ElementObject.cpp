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
#include "ElementObject.h"
#include "WorldObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("ElementObject");

ElementProvider::ElementProvider(const Handler<Heap>& heap, const Handler<World>& world)
:Super(heap, "chisa::tk::ElementObject")
,world_(world)
{
}

Handler<World> ElementProvider::world() const
{
	return world_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ElementObject::ElementObject(ElementProvider* provider)
:ReactiveNativeObject(provider)
,world_(provider->world())
{
}

void ElementObject::bootstrap(const Handler<Heap>& heap)
{
	this->ReactiveNativeObject::bootstrap(heap);
}

std::string ElementObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(ElementObject: %p)", this);
}

XValue ElementObject::onBack(const Handler<Heap>& heap, const XValue& val)
{
	//XXX
	return XValue();
}

XValue ElementObject::onForward(const Handler<Heap>& heap, const XValue& val)
{
	//XXX
	return XValue();
}

XValue ElementObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void ElementObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

}}
