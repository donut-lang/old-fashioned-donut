/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <functional>
#include "WorldObject.hpp"
#include "ElementObject.hpp"
#include "../Heaven.hpp"
#include "../element/WidgetElement.hpp"

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
	this->registerReactiveNativeClosure("popElement", &WorldObject::popElement);
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
	return ::cinamo::format("(WorldObject: %p)", this);
}

Handler<World> WorldObject::world() const
{
	return this->world_.lock();
}

typename WorldObject::ResultType WorldObject::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::PushElement:
		anti.op = AntiSideEffect::PopElement;
		this->world()->pushElement(val.elm);
		break;
	case AntiSideEffect::PopElement:
		anti.op = AntiSideEffect::PushElement;
		anti.elm = this->world()->rootElement();
		this->world()->popElement();
		break;
	}

	return Just<AntiSideEffect>(anti);
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


std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> > WorldObject::pushElement(std::string const& elementId)
{
	Handler<World> const world = this->world();
	WorldSideEffect anti;
	anti.op = AntiSideEffect::PopElement;
	world->pushElement(elementId);
	return std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> >(world->rootElement()->donutObject(), Just(anti));
}
std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> > WorldObject::popElement()
{
	Handler<World> const world = this->world();
	WorldSideEffect anti;
	anti.op = AntiSideEffect::PushElement;
	anti.elm = world->rootElement();
	world->popElement();
	return std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> >(anti.elm->donutObject(), Just(anti));

}

}}
