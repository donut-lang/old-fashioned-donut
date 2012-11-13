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
#include "LayoutFactory.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(Empty)
{

}

Empty::~Empty() noexcept
{
}

weak_ptr<Element> Empty::getChildAt(const size_t index) const
{
	return weak_ptr<Element>();
}
size_t Empty::getChildCount() const
{
	return 0;
}

void Empty::loadXMLimpl(LayoutFactory* const factory, tinyxml2::XMLElement* element)
{

}

string Empty::toString() const
{
	return util::format("(EmptyLayout)");
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

weak_ptr<Element> Empty::getLayoutByIdImpl(const std::string& id)
{
	return weak_ptr<Element>();
}

}}}
