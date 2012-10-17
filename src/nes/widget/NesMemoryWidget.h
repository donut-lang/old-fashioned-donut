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
	std::weak_ptr<nes::NesGeist> geist_;
public:
	NesMemoryWidget(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> _world, tinyxml2::XMLElement* element);
	virtual ~NesMemoryWidget();
public:
	virtual void render(chisa::gl::Canvas& cv, const chisa::geom::Area& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(const chisa::geom::Box& areaSize) override;
	virtual chisa::geom::Box measure(const chisa::geom::Box& constraintSize) override;
};

}}

