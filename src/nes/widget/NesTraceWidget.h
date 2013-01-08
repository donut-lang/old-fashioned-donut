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

#include <tarte/Handler.h>
#include <tarte/XVal.h>
#include "../../chisa/Hexe.h"
#include "../../chisa/tk/Widget.h"
#include "../../chisa/gl/PredefinedSymRenderer.h"

namespace nes {
using namespace tarte;

class NesGeist;

class NesTraceWidget: public ::chisa::tk::Widget {
private:
	chisa::HandlerW<nes::NesGeist> geist_;
	chisa::gl::PredefinedSymRenderer asmRenderer_;
	chisa::gl::PredefinedSymRenderer numRenderer_;
	enum {
		Dollar    , // "$"
		RightParen, // "("
		LeftParen , // ")"
		Eq        , // "="
		Comma     , //","
		AtMark    , //"@"
		SymX      , //"X"
		SymY      , //"Y"
	};
	chisa::gl::PredefinedSymRenderer symRenderer_;
	float addrWidth_;
public:
	NesTraceWidget(Logger& log, chisa::HandlerW<chisa::tk::World> world, tinyxml2::XMLElement* element);
	virtual ~NesTraceWidget() noexcept = default;
public:
	virtual void render(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measure(chisa::geom::Box const& constraintSize) override;
};

}
