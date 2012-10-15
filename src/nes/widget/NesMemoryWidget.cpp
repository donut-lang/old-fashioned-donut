/*
 * NesMemoryWidget.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: psi
 */

#include "NesMemoryWidget.h"
#include "../../chisa/tk/World.h"
#include "../NesGeist.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"

namespace nes {
namespace widget {

NesMemoryWidget::NesMemoryWidget(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
{
	std::shared_ptr<chisa::tk::World> world(_world.lock());
	std::shared_ptr<nes::NesGeist> geist(std::dynamic_pointer_cast<nes::NesGeist>(world->geist()));
	this->geist_ = geist;
}

NesMemoryWidget::~NesMemoryWidget()
{
}

void NesMemoryWidget::render(chisa::gl::Canvas& cv, const chisa::geom::Area& area)
{
}

void NesMemoryWidget::idle(const float delta_ms)
{
}

void NesMemoryWidget::reshape(const chisa::geom::Box& areaSize)
{
}

chisa::geom::Box NesMemoryWidget::measure(const chisa::geom::Box& constraintSize)
{
	return constraintSize;
}

}}
