/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "AngelElementTarget.h"
#include "../World.h"
#include "../Element.h"
#include "../donut/Patron.h"

namespace chisa {
namespace tk {

AngelElementTarget::AngelElementTarget(const Handler<Angel>& angel, const std::string& id)
:AngelTarget(angel)
,id_(id)
{
}

Handler<Element> AngelElementTarget::findElement()
{
	Handler<World> const world = this->world();
	if( unlikely(!world) ) {
		return Handler<Element>();
	}
	return world->findElementById(id_);
}
geom::Area AngelElementTarget::findScreenAreaImpl()
{
	Handler<Element> const element = findElement();
	if( unlikely(!element) ) {
		return geom::Area();
	}
	return element->lastDrawnAreaInRoot();
}

Handler<AngelElementTarget> AngelElementTarget::matchToElementTarget(const std::string& elementId) noexcept
{
	return (this->id_ == elementId) ?
			Handler<AngelElementTarget>::__internal__fromRawPointerWithoutCheck(this) :
			Handler<AngelElementTarget>();
}

Handler<AngelElementTarget> AngelElementTarget::self() noexcept
{
	return Handler<AngelElementTarget>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> AngelElementTarget::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->angelElementTargetProvider()->newInstance(heap, self()) );
}

void AngelElementTarget::renderImpl(gl::Canvas& canvas)
{
}

void AngelElementTarget::idleImpl(const float delta_ms)
{
}

void AngelElementTarget::onAttatchedImpl()
{
	Handler<World> const world = this->world();
	if( unlikely(!world) ) {
		return;
	}
	Handler<Element> const element = world->findElementById(id_);
	if( unlikely(!element) ) {
		return;
	}
	element->showPoint( geom::ZERO );
}

geom::Box AngelElementTarget::reshapeImpl(const geom::Area& area)
{
	return geom::ZERO;
}



Handler<AngelElementTarget> AngelElementTarget::toElementTarget()
{
	return self();
}

}}

