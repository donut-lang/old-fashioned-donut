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

#pragma once

#include "../../chisa/tk/Widget.h"
#include "../../chisa/Hexe.h"
#include "../../chisa/util/XVal.h"

namespace nes {
class NesGeist;

namespace widget {

class NesScreenWidget: public ::chisa::tk::Widget {
private:
	chisa::HandlerW<nes::NesGeist> geist_;
	chisa::Handler<chisa::util::XObject> conf_;
public:
	NesScreenWidget(chisa::logging::Logger& log, chisa::HandlerW<chisa::tk::World> world, tinyxml2::XMLElement* element);
	virtual ~NesScreenWidget();
public:
	virtual void render(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measure(chisa::geom::Box const& constraintSize) override;
};

}}
