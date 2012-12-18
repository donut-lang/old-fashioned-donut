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
#include "../../chisa/gl/DrawableManager.h"

namespace nes {

NesMemoryWidget::NesMemoryWidget(chisa::logging::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;
	chisa::Handler<chisa::gl::DrawableManager> mgr = world->drawableManager();
	chisa::Handler<chisa::gl::Font> font = mgr->queryFont();
	chisa::gl::Font::RawFaceSession s(font);
}

NesMemoryWidget::~NesMemoryWidget()
{
}

void NesMemoryWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
}

void NesMemoryWidget::idle(const float delta_ms)
{
}

void NesMemoryWidget::reshape(chisa::geom::Box const& areaSize)
{
}

chisa::geom::Box NesMemoryWidget::measure(chisa::geom::Box const& constraintSize)
{
	return constraintSize;
}

}
