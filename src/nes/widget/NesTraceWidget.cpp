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

#include "NesTraceWidget.h"
#include "../NesGeist.h"
#include "../../chisa/tk/World.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"

namespace nes {

NesTraceWidget::NesTraceWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,asmRenderer_(log, _world.lock()->drawableManager(), 24.0f)
,addrWidth_(0)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;
}

NesTraceWidget::~NesTraceWidget()
{
}

void NesTraceWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
}

void NesTraceWidget::idle(const float delta_ms)
{
}

void NesTraceWidget::reshape(chisa::geom::Box const& areaSize)
{

}

chisa::geom::Box NesTraceWidget::measure(chisa::geom::Box const& constraintSize)
{
	return chisa::geom::Box(512, chisa::geom::Unspecified);
}

}
