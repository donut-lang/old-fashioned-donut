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

#include "SplitLayout.h"
#include "LayoutFactory.h"

namespace chisa {
namespace tk {
namespace layout {

SplitLayout::SplitLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:Layout(log, world, root, parent)
{

}


SplitLayout::~SplitLayout() {
	// TODO Auto-generated destructor stub
}

void SplitLayout::addChild(const SplitDef& def, shared_ptr<Layout> layout)
{
	shared_ptr<SplitCtx> ctx(new SplitCtx(def));
	ctx->layout = layout;
	this->children_.push_back(ctx);
}

void SplitLayout::loadTree(LayoutFactory& factory, XMLElement* top)
{
	for(XMLNode* _node = top->FirstChild(); _node!= top->LastChild(); _node=_node->NextSibling()){
		XMLElement* elem = _node->ToElement();
		if(!elem){
			continue;
		}
		float weight = NAN;
		float min = NAN;
		float max = NAN;
		elem->QueryFloatAttribute("weight", &weight);
		elem->QueryFloatAttribute("min", &min);
		elem->QueryFloatAttribute("max", &max);
		const SplitDef def(weight, min, max);
		this->addChild(def, factory.parseTree(this->root(), this->parent(), elem));
	}
}

void SplitLayout::idle(const float delta_ms)
{
	for(shared_ptr<SplitCtx> ctx : this->children()){
		ctx->layout->idle(delta_ms);
	}
}

void SplitLayout::reshapeLinear(const float size)
{

}

weak_ptr<Layout> SplitLayout::getChildAt(const size_t index) const
{
	if(index < this->children_.size()){
		shared_ptr<SplitCtx> ctx = this->children_.at(index);
		return weak_ptr<Layout>(ctx->layout);
	}
	return weak_ptr<Layout>();
}
size_t SplitLayout::getChildCount() const
{
	return this->children_.size();
}
weak_ptr<Widget> SplitLayout::getWidgetById(const string& id)
{

}


}}}
