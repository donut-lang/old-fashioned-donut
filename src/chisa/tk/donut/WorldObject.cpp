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
#include "../Heaven.h"
#include "../element/WidgetElement.h"

namespace chisa {
namespace tk {

const static std::string TAG("WorldObject");

WorldProvider::WorldProvider(const Handler<Heap>& heap, const Handler<World>& world)
:Super(heap, "chisa::tk::WorldObject")
,world_(world)
{
	this->registerPureNativeClosure("findElementById", &WorldObject::findElementById);
	this->registerPureNativeClosure("heaven", &WorldObject::heaven);
	this->registerPureNativeClosure("findWidgetById", &WorldObject::findWidgetById);
	this->registerReactiveNativeClosure("pushElement", &WorldObject::pushElement);
}

Handler<World> WorldProvider::world() const
{
	return world_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

WorldObject::WorldObject(WorldProvider* provider)
:Super(provider)
{
}

void WorldObject::bootstrap(const Handler<Heap>& heap, Handler<World> const& world)
{
	this->ReactiveNativeObject::bootstrap(heap);
	this->world_ = world;
}

std::string WorldObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(WorldObject: %p)", this);
}

Handler<World> WorldObject::world() const
{
	return this->world_.lock();
}

WorldObject::ResultType WorldObject::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::PushElement:
		anti.op = AntiSideEffect::PushElement;
		anti.elm = this->world()->rootElement();
		this->world()->pushElement(val.elm);
		break;
	}

	return std::tuple<bool, AntiSideEffect>(true, anti);
}

WorldObject::ResultType WorldObject::onBack(const Handler<Heap>& heap, const WorldSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

WorldObject::ResultType WorldObject::onForward(const Handler<Heap>& heap, const WorldSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

XValue WorldObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void WorldObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void WorldObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

void WorldObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

Handler< ::donut::Object> WorldObject::heaven( Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args )
{
	Handler<World> const world = self.cast<WorldObject>()->world();
	return world->heaven()->donutObject(heap);
}

Handler<ElementObject> WorldObject::findElementById(const std::string& id)
{
	Handler<World> const world = this->world();
	return world->findElementById(id)->donutObject();
}

Handler<WidgetObject> WorldObject::findWidgetById(const std::string& widgetid)
{
	Handler<World> const world = this->world();
	Handler<WidgetElement> elm(world->findWidgetById(widgetid));
	if( !elm ) {
		return Handler<WidgetObject>();
	}
	return elm->widget()->donutObject();
}


std::tuple<Handler<ElementObject>, bool, WorldSideEffect> WorldObject::pushElement(std::string const& elementId)
{
	Handler<World> const world = this->world();
	WorldSideEffect anti;
	anti.op = AntiSideEffect::PushElement;
	anti.elm = world->rootElement();
	world->pushElement(elementId);
	return std::tuple<Handler<ElementObject>, bool, WorldSideEffect>(world->rootElement()->donutObject(), true, anti);
}

}}
