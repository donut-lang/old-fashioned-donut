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

#ifndef _CXX_Chisa_HEXE_H_
#define _CXX_Chisa_HEXE_H_

#include "../chisa/Hexe.h"

namespace nes {

class Hexe: public chisa::Hexe {
public:
	Hexe(chisa::logging::Logger& log, const std::string& basepath);
	virtual ~Hexe();
public:
	virtual void registerWidgets(chisa::tk::widget::WidgetFactory& factory) override;
	virtual void registerLayouts(chisa::tk::layout::LayoutFactory& factory) override;
public:
	virtual std::shared_ptr<chisa::WorldGeist> invokeWorldGeist(std::weak_ptr<chisa::tk::World> world, const std::string& nameOfGeist) override;
};

}
#endif /* INCLUDE_GUARD */