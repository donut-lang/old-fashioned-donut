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

#include "AngelWidgetTarget.h"
#include "../Element.h"
#include "../element/WidgetElement.h"
#include "../Widget.h"
#include "../World.h"
#include "../donut/Patron.h"

namespace chisa {
namespace tk {


AngelWidgetTarget::AngelWidgetTarget(const Handler<Angel>& angel, const std::string& id, const std::string& guide)
:AngelTarget(angel)
,id_(id)
,guide_(guide)
{
}

geom::Area AngelWidgetTarget::findScreenAreaImpl()
{
	Handler<World> const world = this->world();
	if( unlikely(!world) ) {
		return geom::Area();
	}
	Handler<WidgetElement> const element = world->getWidgetById(id_);
	if( unlikely(!element) ) {
		return geom::Area();
	}
	Handler<Widget> const& widget = element->widget();
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
	Handler<WidgetElement> const element = world->getWidgetById(id_);
	if( unlikely(!element) ) {
		return;
	}
	geom::Area area(element->widget()->findTargetInElement(guide_));
	geom::Point center(area.point() + area.box()/2);
	if( !area.empty() && !element->lastInnerDrawnAreaInRoot().contain(center+element->lastPositionInRoot()) ) {
		element->showPoint(center);
	}
}

geom::Box AngelWidgetTarget::reshapeImpl(const geom::Area& area)
{
	return geom::ZERO;
}

}}

