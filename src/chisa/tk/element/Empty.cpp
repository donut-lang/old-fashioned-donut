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

#include "Empty.h"
#include "ElementFactory.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Empty, LeafElement)
{

}

Empty::~Empty() noexcept
{
}


void Empty::loadXMLimpl(ElementFactory* const factory, tinyxml2::XMLElement* element)
{

}

std::string Empty::toString() const
{
	return util::format("(Empty %p)",this);
}

void Empty::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	//何も描画しない
}

geom::Box Empty::onMeasure(const geom::Box& constraint)
{
	return geom::Box(geom::Unspecified, geom::Unspecified);
}

void Empty::onLayout(const geom::Box& size)
{
	//何もしない
}

}}}
