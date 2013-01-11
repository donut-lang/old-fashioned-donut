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
#include "heaven/AngelElementTarget.h"
#include "heaven/AngelWidgetTarget.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {


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

AngelTarget::AngelTarget(const Handler<Angel>& angel)
:world_(angel->world())
,heaven_(angel->heaven())
,angel_(angel)
{
}

geom::Area AngelTarget::findScreenArea()
{
	return this->findScreenAreaImpl();
}

Handler<World> AngelTarget::world() const
{
	return this->world_.lock();
}
Handler<Heaven> AngelTarget::heaven() const
{
	return this->heaven_.lock();
}
Handler<Angel> AngelTarget::angel() const
{
	return this->angel_.lock();
}

void AngelTarget::attatchServant(const Handler<Servant>& servant)
{
	this->servants_.push_back(servant);
}

/**********************************************************************************************************************
 * Servants
 **********************************************************************************************************************/

Servant::Servant(Handler<AngelTarget> const& angel_target)
:world_(angel_target->world())
,heaven_(angel_target->heaven())
,angel_(angel_target->angel())
,target_(angel_target)
{

}

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
Handler<AngelTarget> Servant::target() const
{
	return this->target_.lock();
}

void Angel::registerServantToWidget(const std::string& widgetId, const std::string& widgetGuide, Handler<Servant> servant)
{
	for(Handler<AngelTarget> const& target : this->targets_) {
		if(target->matchToWidgetTarget(widgetId, widgetGuide)) {
			target->attatchServant(servant);
			return;
		}
	}
	Handler<AngelWidgetTarget> target (new AngelWidgetTarget(self(), widgetId, widgetGuide));
	target->attatchServant(servant);
	this->targets_.push_back(target);
}

void Angel::registerServantToElement(const std::string& elementId, Handler<Servant> servant)
{
	for(Handler<AngelTarget> const& target : this->targets_) {
		if(target->matchToElementTarget(elementId)) {
			target->attatchServant(servant);
			return;
		}
	}
	Handler<AngelElementTarget> target (new AngelElementTarget(self(), elementId));
	target->attatchServant(servant);
	this->targets_.push_back(target);
}

}}

