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
#include "AngelElementTarget.h"
#include "../World.h"
#include "../Element.h"

namespace chisa {
namespace tk {

AngelElementTarget::AngelElementTarget(const Handler<Angel>& angel, const std::string& id)
:AngelTarget(angel)
,id_(id)
{
}

geom::Area AngelElementTarget::findScreenAreaImpl()
{
	Handler<World> world = this->world();
	if( unlikely(!world) ) {
		return geom::Area();
	}
	Handler<Element> element = world->findElementById(id_);
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

Handler< ::donut::Object> AngelElementTarget::createDonutObject(Handler< ::donut::Heap> const& heap)
{
}

}}

