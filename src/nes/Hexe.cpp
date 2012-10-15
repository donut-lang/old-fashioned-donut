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
#include "NesGeist.h"

namespace nes {

Hexe::Hexe(chisa::logging::Logger& log, const std::string& basepath)
:chisa::Hexe(log, basepath)
{

}

Hexe::~Hexe()
{
}

void Hexe::registerWidgets(chisa::tk::widget::WidgetFactory& factory)
{
	factory.registerWidget<nes::widget::NesScreenWidget>("nes-screen");
}

void Hexe::registerLayouts(chisa::tk::layout::LayoutFactory& factory)
{
}

std::shared_ptr<chisa::WorldGeist> Hexe::invokeWorldGeist(std::weak_ptr<chisa::tk::World> world, const std::string& nameOfGeist)
{
	if(nameOfGeist == "nes"){
		return std::shared_ptr<chisa::WorldGeist>(new NesGeist(log(), world));
	}
	return std::shared_ptr<chisa::WorldGeist>();
}

}
