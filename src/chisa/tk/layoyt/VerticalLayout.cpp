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

#include "VerticalLayout.h"

namespace chisa {
namespace tk {
namespace layout {

VerticalLayout::VerticalLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:SplitLayout(log, world, root, parent)
{
	// TODO Auto-generated constructor stub

}

VerticalLayout::~VerticalLayout() {
	// TODO Auto-generated destructor stub
}

string VerticalLayout::toString()
{
	return util::format("(VerticalLayout %p)", this);
}

void VerticalLayout::render(const Area& area)
{

}
void VerticalLayout::reshape(const Box& area)
{

}

}}}
