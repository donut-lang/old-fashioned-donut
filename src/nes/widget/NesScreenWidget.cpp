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
namespace widget {

NesScreenWidget::NesScreenWidget(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
{
	std::shared_ptr<chisa::tk::World> world(_world.lock());
	std::shared_ptr<nes::NesGeist> geist(std::dynamic_pointer_cast<nes::NesGeist>(world->geist()));
	this->geist_ = geist;
	this->params_.parseTree(element);

	std::string rom;
	if(this->params_.queryString("rom", &rom)){
		geist->loadNES(world->resolveUniverseFilepath(rom));
		geist->startNES();
	}
}

NesScreenWidget::~NesScreenWidget()
{
}

void NesScreenWidget::render(chisa::gl::Canvas& cv, const chisa::geom::Area& area)
{
	std::shared_ptr<nes::NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	nes::NesGeist::Lock lock(*geist.get());
	chisa::gl::Canvas::AffineScope as(cv);
	{
		cv.translate(-area.point());
		cv.drawSprite(lock.getSprite(), chisa::geom::Vector(0.0f, 0.0f), 0.0f);
	}
}

void NesScreenWidget::idle(const float delta_ms)
{
}

void NesScreenWidget::reshape(const chisa::geom::Box& areaSize)
{

}

chisa::geom::Box NesScreenWidget::measure(const chisa::geom::Box& constraintSize)
{
	return chisa::geom::Box(256,240);
}

}}
