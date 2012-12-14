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

#include "Hexe.h"
#include "widget/NesScreenWidget.h"
#include "widget/NesMemoryWidget.h"
#include "NesGeist.h"

namespace nes {

Hexe::Hexe(chisa::logging::Logger& log, std::string const& basepath)
:chisa::Hexe(log, basepath)
{

}

Hexe::~Hexe() noexcept
{
}

void Hexe::registerWidgets(chisa::tk::widget::WidgetFactory& factory)
{
	factory.registerWidget<nes::widget::NesScreenWidget>("nes-screen");
	factory.registerWidget<nes::widget::NesMemoryWidget>("nes-memory");
}

void Hexe::registerElements(chisa::tk::element::ElementFactory& factory)
{
}

chisa::Handler<chisa::WorldGeist> Hexe::invokeWorldGeist(chisa::HandlerW<chisa::tk::World> world, std::string const& nameOfGeist)
{
	if(nameOfGeist == "nes"){
		return chisa::Handler<chisa::WorldGeist>(new NesGeist(log(), world));
	}
	return chisa::Handler<chisa::WorldGeist>();
}

}
