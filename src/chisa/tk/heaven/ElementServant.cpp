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

#include "ElementServant.h"
#include "../Element.h"
#include "../World.h"
#include "../donut/Patron.h"

namespace chisa {
namespace tk {

ElementServant::ElementServant(Handler<AngelTarget> const& angelTarget)
:Servant(angelTarget)
{

}

Handler<ElementServant> ElementServant::self() noexcept
{
	return Handler<ElementServant>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> ElementServant::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->elementServantProvider()->newInstance(heap, self()) );
}

}}

