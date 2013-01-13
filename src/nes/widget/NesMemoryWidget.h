/*
 * NesMemoryWidget.h
 *
 *  Created on: Oct 15, 2012
 *      Author: psi
 */

#pragma once

#include "../../chisa/gl/PredefinedSymRenderer.h"
#include "../../chisa/tk/Widget.h"

namespace nes {
class NesGeist;

class NesMemoryWidget: public chisa::tk::Widget {
private:
	chisa::HandlerW<nes::NesGeist> geist_;
	chisa::gl::PredefinedSymRenderer numRenderer_;
	float addrWidth_;
public:
	NesMemoryWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element);
	virtual ~NesMemoryWidget() noexcept = default;
public:
	virtual void renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idleImpl(const float delta_ms) override;
	virtual void reshapeImpl(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measureImpl(chisa::geom::Box const& constraintSize) override;
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
private:
	chisa::geom::Area addrToArea(uint16_t const& addr);
	uint16_t ptToAddr(chisa::geom::Point const& pt);
private:
	virtual bool onSingleTapUp(float const& timeMs, chisa::geom::Point const& ptInWidget) override;
};

}
