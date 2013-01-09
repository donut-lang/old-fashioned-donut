/**
 * Chisa
 * Copyright (C) 2013 psi
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
#include "../../chisa/gl/Drawable.h"
#include "../../chisa/gl/PredefinedSymRenderer.h"

namespace nes {
using namespace tarte;

class NesGeist;
class NesMemoryCompare final : public ::chisa::tk::Widget {
private:
	Handler<chisa::gl::TextDrawable> lAddr_; // "アドレス"
	Handler<chisa::gl::TextDrawable> lLast_; // "前の値"
	Handler<chisa::gl::TextDrawable> lNow_; // "今の値"
	chisa::gl::PredefinedSymRenderer numRenderer_;
	static const constexpr unsigned int Dollar=16;
	static const constexpr unsigned int HexStart=17;
	static const constexpr unsigned int Eq=18;
	static const constexpr unsigned int RParen=19;
	static const constexpr unsigned int LParen=20;
	static const constexpr unsigned int Space=21;
	float width_;
	float addrWidth_;
	float lastWidth_;
	float nowWidth_;
private:
	chisa::HandlerW<nes::NesGeist> geist_;
public:
	NesMemoryCompare(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element);
	virtual ~NesMemoryCompare() noexcept = default;
private:
	inline float rowHeight() const noexcept { return this->numRenderer_.maxHeight(); };
private:
	virtual void render(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override final;
	virtual void idle(const float delta_ms) override final;
	virtual void reshape(chisa::geom::Box const& areaSize) override final;
	virtual chisa::geom::Box measure(chisa::geom::Box const& constraintSize) override final;
};

}
