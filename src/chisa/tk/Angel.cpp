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

Handler<AngelElementTarget> AngelTarget::matchToElementTarget(std::string const& elementId) noexcept
{
	return Handler<AngelElementTarget>();
}
Handler<AngelWidgetTarget> AngelTarget::matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept {
	return Handler<AngelWidgetTarget>();
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

Handler<AngelWidgetTarget> Angel::newWidgetTarget(std::string const& widgetId, std::string const& widgetGuide)
{
	return Handler<AngelWidgetTarget>(new AngelWidgetTarget(self(), widgetId, widgetGuide));
}
Handler<AngelElementTarget> Angel::newElementTarget(std::string const& elementId)
{
	return Handler<AngelElementTarget>(new AngelElementTarget(self(), elementId));
}

Handler<AngelWidgetTarget> Angel::findWidgetTarget(const std::string& widgetId, const std::string& widgetGuide )
{
	Handler<AngelWidgetTarget> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		if((res = target->matchToWidgetTarget(widgetId, widgetGuide))) {
			return res;
		}
	}
	return res;
}

Handler<AngelElementTarget> Angel::findElementTarget(const std::string& elementId )
{
	Handler<AngelElementTarget> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		if((res = target->matchToElementTarget(elementId))) {
			return res;
		}
	}
	return res;
}

}}

