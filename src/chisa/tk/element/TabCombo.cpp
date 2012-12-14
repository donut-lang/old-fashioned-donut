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

#include "TabCombo.h"
#include <tinyxml2.h>
#include "../../gl/Canvas.h"
#include "../World.h"
#include "../../logging/Exception.h"
#include "../../gl/DrawableManager.h"

#include <iostream>
namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TabCombo, LeafElement)
{
}

TabCombo::~TabCombo() noexcept
{
}

std::string TabCombo::toString() const
{
	return util::format("(TabCombo %p)", this);
}

void TabCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
}
geom::Box TabCombo::onMeasure(geom::Box const& constraint)
{
}

void TabCombo::onLayout(geom::Box const& size)
{
}

void TabCombo::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

}}}
