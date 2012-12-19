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

#include "../Element.h"
#include "AbstractButton.h"

namespace chisa {
namespace tk {

class Button: public AbstractButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(Button);
public:
	struct AttrName{
		const static std::string ShadowColor;
		const static std::string ShadowDepth;
	};
private:
	gl::Color shadowColor_;
	float shadowDepth_;
	geom::Box renderOffset_;
public:
	virtual std::string toString() const override final;
	inline gl::Color const& shadowColor() const noexcept { return this->shadowColor_; };
	inline float shadowDepth() const noexcept { return this->shadowDepth_; };
	void shadowColor(gl::Color const& color);
	void shadowDepth(float const& depth);
private:
	virtual geom::Box measureButtonContent(geom::Box const& constraint) override final;
	virtual void layoutButtonContent(geom::Box const& size) override final;
	virtual void renderOn(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override final;
	virtual void renderOff(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override final;
	virtual bool isOn() const noexcept override final;
};

}}
