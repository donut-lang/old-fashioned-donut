/*
 * NesMemoryWidget.h
 *
 *  Created on: Oct 15, 2012
 *      Author: psi
 */

#pragma once

#include "../../chisa/tk/Widget.h"

namespace nes {
class NesGeist;

namespace widget {

class NesMemoryWidget: public chisa::tk::Widget {
private:
	chisa::HandlerW<nes::NesGeist> geist_;
public:
	NesMemoryWidget(chisa::logging::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element);
	virtual ~NesMemoryWidget();
public:
	virtual void render(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measure(chisa::geom::Box const& constraintSize) override;
};

}}

