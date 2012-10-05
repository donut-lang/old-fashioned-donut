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

const std::string SplitLayout::AttrName::Weight("split-weight");
const std::string SplitLayout::AttrName::Max("split-max");
const std::string SplitLayout::AttrName::Min("split-min");

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(SplitLayout)
,totalSize_(geom::Unspecified)
{

}

SplitLayout::~SplitLayout() {
}

void SplitLayout::addChild(const SplitDef& def, shared_ptr<Layout> layout)
{
	shared_ptr<SplitCtx> ctx(new SplitCtx(def));
	ctx->layout = layout;
	this->children_.push_back(ctx);
}

void SplitLayout::loadXML(LayoutFactory* const factory, XMLElement* top)
{
	for(XMLNode* _node = top->FirstChild(); _node; _node=_node->NextSibling()){
		XMLElement* elem = _node->ToElement();
		if(!elem){
			continue;
		}
		float weight = NAN;
		float min = NAN;
		float max = NAN;
		elem->QueryFloatAttribute(AttrName::Weight.c_str(), &weight);
		elem->QueryFloatAttribute(AttrName::Min.c_str(), &min);
		elem->QueryFloatAttribute(AttrName::Max.c_str(), &max);
		const SplitDef def(weight, min, max);
		this->addChild(def, factory->parseTree(this->root(), this->parent(), elem));
	}
}

void SplitLayout::idle(const float delta_ms)
{
	for(shared_ptr<SplitCtx> ctx : this->children()){
		ctx->layout->idle(delta_ms);
	}
}

void SplitLayout::resetChildrenLayout()
{
	for(shared_ptr<SplitCtx> ctx : this->children()){
		ctx->size=geom::Unspecified;
	}
	this->totalSize_ = geom::Unspecified;
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

}}}
