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

#include "NesScreenWidget.h"
#include "../NesGeist.h"
#include "../../chisa/tk/World.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"

namespace nes {

NesScreenWidget::NesScreenWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,conf_(new XObject)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;
	this->conf_->accumlate(element->FirstChildElement());

	if(this->conf_->has<XString>("rom")){
		geist->loadNES(world->resolveUniverseFilepath(this->conf_->get<XString>("rom")));
		geist->startNES();
	}
}

void NesScreenWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	nes::NesGeist::Lock lock(*geist.get());
	chisa::gl::Canvas::AffineScope as(cv);
	{
		cv.translate(-area.point());
		cv.drawSprite(lock.getSprite(), chisa::geom::Vector(0.0f, 0.0f));
	}
}

void NesScreenWidget::idle(const float delta_ms)
{
}

void NesScreenWidget::reshape(chisa::geom::Box const& areaSize)
{

}

chisa::geom::Box NesScreenWidget::measure(chisa::geom::Box const& constraintSize)
{
	return chisa::geom::Box(256,240);
}

}
