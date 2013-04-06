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
#include <cinamo/Handler.h>
#include <cinamo/XVal.h>

namespace nes {
using namespace cinamo;

class NesGeist;

class NesScreenWidget: public ::chisa::tk::Widget {
private:
	HandlerW<nes::NesGeist> geist_;
	Handler<XObject> conf_;
	bool lastState_;
public:
	NesScreenWidget(Logger& log, chisa::HandlerW<chisa::tk::World> world, tinyxml2::XMLElement* element);
	virtual ~NesScreenWidget() noexcept = default;
public:
	virtual void onShownImpl() override final;
	virtual void onHiddenImpl() override final;
	virtual void renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idleImpl(const float delta_ms) override;
	virtual void reshapeImpl(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measureImpl(chisa::geom::Box const& constraintSize) override;
	virtual void onFocusGained(float const& timeMs, chisa::geom::Point const& lastPtInScreen) override;
	virtual void onFocusLost(float const& timeMs) override;
	virtual bool onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym) override final;
	virtual bool onKeyUp(float const& timeMs, SDL_Keysym const& sym) override final;
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
};

}
