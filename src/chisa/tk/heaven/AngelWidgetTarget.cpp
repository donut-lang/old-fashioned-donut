/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "AngelWidgetTarget.hpp"
#include "../Element.hpp"
#include "../element/WidgetElement.hpp"
#include "../Widget.hpp"
#include "../World.hpp"
#include "../donut/Patron.hpp"

namespace chisa {
namespace tk {


AngelWidgetTarget::AngelWidgetTarget(const Handler<Angel>& angel, const std::string& id, const std::string& guide)
:AngelTarget(angel)
,id_(id)
,guide_(guide)
{
}
Handler<WidgetElement> AngelWidgetTarget::findElement()
{
	Handler<World> const world = this->world();
	if( unlikely(!world) ) {
		return Handler<WidgetElement>();
	}
	return world->findWidgetById(id_);
}

Handler<Widget> AngelWidgetTarget::findWidget()
{
	Handler<WidgetElement> const element = findElement();
	if( unlikely(!element) ) {
		return Handler<Widget>();
	}
	return element->widget();
}

geom::Area AngelWidgetTarget::findScreenAreaImpl()
{
	Handler<Widget> const widget = findWidget();
	if( unlikely(!widget) ) {
		return geom::Area();
	}
	return widget->findTargetInRoot(guide_);
}

Handler<AngelWidgetTarget> AngelWidgetTarget::matchToWidgetTarget(const std::string& widgetId, const std::string& widgetGuide) noexcept
{
	return
			(this->id_ == widgetId && this->guide_ == widgetGuide) ?
					Handler<AngelWidgetTarget>::__internal__fromRawPointerWithoutCheck(this) :
					Handler<AngelWidgetTarget>();
}
Handler<AngelWidgetTarget> AngelWidgetTarget::self() noexcept
{
	return Handler<AngelWidgetTarget>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> AngelWidgetTarget::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->angelWidgetTargetProvider()->newInstance(heap, self()) );
}

void AngelWidgetTarget::renderImpl(gl::Canvas& canvas)
{
}

void AngelWidgetTarget::idleImpl(const float delta_ms)
{
}

void AngelWidgetTarget::onAttatchedImpl()
{
	Handler<World> world = this->world();
	if( unlikely(!world) ) {
		return;
	}
	Handler<WidgetElement> const element = world->findWidgetById(id_);
	if( unlikely(!element) ) {
		return;
	}
	geom::Area area(element->widget()->findTargetInElement(guide_));
	geom::Point center( area.center() );
	if( !area.empty() && !element->lastInnerDrawnAreaInRoot().contain(center+element->lastPositionInRoot()) ) {
		element->showPoint(center);
	}
}

geom::Box AngelWidgetTarget::reshapeImpl(const geom::Area& area)
{
	return geom::ZERO;
}

Handler<AngelWidgetTarget> AngelWidgetTarget::toWidgetTarget()
{
	return self();
}

}}
