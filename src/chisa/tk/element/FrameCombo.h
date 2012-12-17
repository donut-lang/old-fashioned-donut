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
#include "../../geom/Area.h"

namespace chisa {
namespace tk {
namespace element {

class FrameCombo : public ElementGroup {
	CHISA_ELEMENT_SUBKLASS(FrameCombo);
public:
	virtual std::string toString() const override;
public:
	std::size_t bringToFront(Handler<Element> const& e);
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
};

}}}
