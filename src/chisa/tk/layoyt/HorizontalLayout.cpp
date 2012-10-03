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

#include "HorizontalLayout.h"
#include "LayoutFactory.h"

namespace chisa {
namespace tk {
namespace layout {

HorizontalLayout::HorizontalLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:SplitLayout(log, world, root, parent)
{
	// TODO Auto-generated constructor stub

}

HorizontalLayout::~HorizontalLayout()
{
}

string HorizontalLayout::toString()
{
	return util::format("(HorizontalLayout %p)", this);
}

HorizontalLayout* HorizontalLayout::parseTree(LayoutFactory& factory, weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top)
{
	HorizontalLayout* layout = new HorizontalLayout(factory.log(), factory.world(), root, parent);
	layout->loadTree(factory, top);
	return layout;
}

void HorizontalLayout::render(const Area& area)
{
	float offset = 0;
	for(SplitCtx& ctx : this->children()){
		const Area target(area.x()+offset, area.y(), ctx.size, area.height());
		offset += ctx.size;
		ctx.layout->render(target);
	}
}

void HorizontalLayout::reshape(const Box& area)
{
	this->reshapeLinear(area.width());
}

}}}
