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

#include "EmptyLayout.h"
#include "LayoutFactory.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace tk {
namespace layout {

EmptyLayout::EmptyLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:Layout(log, world, root, parent)
{
}

EmptyLayout::~EmptyLayout()
{
}

weak_ptr<Layout> EmptyLayout::getChildAt(const size_t index) const
{
	return weak_ptr<Layout>();
}
size_t EmptyLayout::getChildCount() const
{
	return 0;
}
weak_ptr<Widget> EmptyLayout::getWidgetById(const string& id)
{
	return weak_ptr<Widget>();
}
void EmptyLayout::render(const Area& area)
{
	// 空っぽ。何も表示しない。
}
void EmptyLayout::idle(const float delta_ms)
{
	// 何もしない。
}
void EmptyLayout::reshapeImpl(const Area& area)
{
}
Box EmptyLayout::measure(const Box& constraint)
{
	//とりあえず最大を主張しておく
	// FIXME: とりあえず仮置きで「大きい値」
	return Box(
		geom::isUnspecified(constraint.width()) ? geom::VeryLarge : constraint.width(),
		geom::isUnspecified(constraint.height()) ? geom::VeryLarge : constraint.height()
	);
}

void EmptyLayout::loadXML(LayoutFactory* const factory, tinyxml2::XMLElement* element)
{

}

string EmptyLayout::toString()
{
	return util::format("(EmptyLayout)");
}



}}}
