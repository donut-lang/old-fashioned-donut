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

#include "Angel.h"
#include "Heaven.h"
#include "World.h"
#include "../geom/Area.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {

AngelTarget::AngelTarget(const Handler<Angel>& angel)
:world_(angel->world())
,heaven_(angel->heaven())
,angel_(angel)
{
}

geom::Area AngelTarget::findScreenArea()
{
}

Angel::Angel(Handler<Heaven> heaven)
:heaven_(heaven)
{
}

Handler<World> Angel::world() const {
	return this->world_.lock();
}
Handler<Heaven> Angel::heaven() const {
	return this->heaven_.lock();
}

void Angel::render(gl::Canvas& canvas)
{
}

void Angel::idle(const float delta_ms)
{
}

void Angel::reshape(const geom::Area& area)
{
}

/**********************************************************************************************************************
 * Servants
 **********************************************************************************************************************/

Handler<World> Servant::world() const
{
	return this->world_.lock();
}
Handler<Heaven> Servant::heaven() const
{
	return this->heaven_.lock();
}

Handler<Angel> Servant::angel() const
{
	return this->angel_.lock();
}

}}

