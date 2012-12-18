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

#include "ElementGroup.h"
#include "../Task.h"
#include "ElementFactory.h"

namespace chisa {
namespace tk {

class ScrollCombo: public ElementGroupBase<std::nullptr_t> {
	CHISA_ELEMENT_SUBKLASS_FINAL(ScrollCombo);
private:
	static constexpr float ScrollBarTimeOut = 500.0f;
	enum Mode {
		None = 0x00,
		Vertical = 0x01,
		Horizontal = 0x10,
		Both = 0x11
	};
	enum Mode scrollMode_;
	geom::Point scrollOffset_;
	geom::Box childSize_;
	float lastMovedFrom_;
private:
	using Super::addChild;
	virtual void addChild(Handler<Element> const& h, Context const& ctx) override final;
	virtual void addChild(std::size_t const& idx, Handler<Element> const& h, Context const& ctx) override final;
public:
	virtual std::string toString() const override;
	virtual void idle(const float delta_ms) override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
public:
	virtual bool onScroll(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override;
};

}}
