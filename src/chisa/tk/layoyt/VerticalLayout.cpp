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
#include "LayoutFactory.h"

namespace chisa {
namespace tk {
namespace layout {

VerticalLayout::VerticalLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:SplitLayout(log, world, root, parent)
{
}

VerticalLayout::~VerticalLayout()
{
}

string VerticalLayout::toString()
{
	return util::format("(VerticalLayout %p)", this);
}

void VerticalLayout::render(const Area& area)
{
	float offset = 0;
	for(shared_ptr<SplitCtx> ctx : this->children()){
		const Area target(area.x(), area.y()+offset, area.width(), ctx->size);
		offset += ctx->size;
		ctx->layout->render(target);
	}
}
void VerticalLayout::reshape(const Box& area)
{
}

VerticalLayout* VerticalLayout::parseTree(LayoutFactory& factory, weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top)
{
	VerticalLayout* layout = new VerticalLayout(factory.log(), factory.world(), root, parent);
	layout->loadTree(factory, top);
	return layout;
}

Box VerticalLayout::measure(const Box& constraint)
{
	Box box(constraint.width(), geom::Unspecified);
	float width=0.0f;
	float height=0.0f;
	for(shared_ptr<SplitCtx> ctx : this->children()){
		const Box result = ctx->layout->measure(box);
		width = std::max(width, result.width());
		height += result.height();
	}
	return tk::Box(width, height);
}

}}}
