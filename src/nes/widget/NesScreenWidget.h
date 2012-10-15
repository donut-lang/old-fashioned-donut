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

#ifndef _CXX_Chisa_NESSCREENWIDGET_H_
#define _CXX_Chisa_NESSCREENWIDGET_H_

#include "../../chisa/tk/Widget.h"
#include "../../chisa/Hexe.h"
#include "../../chisa/util/Param.h"

namespace nes {
class NesGeist;

namespace widget {

class NesScreenWidget: public ::chisa::tk::Widget {
private:
	std::weak_ptr<nes::NesGeist> geist_;
	chisa::util::ParamSet params_;
public:
	NesScreenWidget(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> world, tinyxml2::XMLElement* element);
	virtual ~NesScreenWidget();
public:
	virtual void render(chisa::gl::Canvas& cv, const chisa::geom::Area& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(const chisa::geom::Box& areaSize) override;
	virtual chisa::geom::Box measure(const chisa::geom::Box& constraintSize) override;
};

}}
#endif /* INCLUDE_GUARD */
