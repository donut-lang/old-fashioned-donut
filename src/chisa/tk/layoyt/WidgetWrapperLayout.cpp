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

#include "WidgetWrapperLayout.h"

namespace chisa {
namespace tk {
namespace layout {

WidgetWrapperLayout::WidgetWrapperLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:Layout(log, world, root, parent)
{
}

WidgetWrapperLayout::~WidgetWrapperLayout()
{
}

WidgetWrapperLayout* WidgetWrapperLayout::parseTree(LayoutFactory& factory, weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top)
{

}
weak_ptr<Layout> WidgetWrapperLayout::getChildAt(const size_t index) const
{

}
size_t WidgetWrapperLayout::getChildCount() const
{
	return 0;
}
weak_ptr<Widget> WidgetWrapperLayout::getWidgetById(const string& id)
{
}
void WidgetWrapperLayout::render(const Area& area)
{

}
void WidgetWrapperLayout::idle(const float delta_ms)
{

}
Box WidgetWrapperLayout::measure(const Box& constraint)
{

}
void WidgetWrapperLayout::reshape(const Box& area)
{
	this->size(area);
}

}}}
