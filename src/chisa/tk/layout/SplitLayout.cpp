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
#include "../Geom.h"

namespace chisa {
namespace tk {
namespace layout {

const std::string SplitLayout::AttrName::Weight("split-weight");
const std::string SplitLayout::AttrName::Max("split-max");
const std::string SplitLayout::AttrName::Min("split-min");


SplitLayout::SplitLayout(enum SplitMode splitMode, CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)
:CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Layout)
,splitMode_(splitMode)
,totalSize_(geom::Unspecified)
,changed_getter(splitMode == Vertical ? (float(Box::*)(void) const)&Box::height : (float(Box::*)(void) const)&Box::width)
,changed_setter(splitMode == Vertical ? (void(Box::*)(float))&Box::height : (void(Box::*)(float))&Box::width)
,fixed_getter((splitMode == Vertical ? (float(Box::*)(void) const)&Box::width : (float(Box::*)(void) const)&Box::height))
,fixed_setter(splitMode == Vertical ? (void(Box::*)(float))&Box::width : (void(Box::*)(float))&Box::height)
,point_getter((splitMode == Vertical ? (float(Point::*)(void) const)&Point::y : (float(Point::*)(void) const)&Point::x))
,point_setter((splitMode == Vertical ? (void(Point::*)(float))&Point::y : (void(Point::*)(float))&Point::x))
{

}

SplitLayout::~SplitLayout() {
}

string SplitLayout::toString()
{
	if(this->splitMode() == Horizontal){
		return util::format("(HorizontalLayout %p)", this);
	}else{
		return util::format("(VerticalLayout %p)", this);
	}
}

void SplitLayout::addChild(const SplitDef& def, shared_ptr<Layout> layout)
{
	shared_ptr<SplitCtx> ctx(new SplitCtx(def));
	ctx->layout = layout;
	this->children_.push_back(ctx);
}

void SplitLayout::loadXMLimpl(LayoutFactory* const factory, XMLElement* top)
{
	for(XMLNode* _node = top->FirstChild(); _node; _node=_node->NextSibling()){
		XMLElement* elem = _node->ToElement();
		if(!elem){
			continue;
		}
		float weight = NAN;
		float min = 0;
		float max = geom::VeryLarge;
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

void SplitLayout::renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area)
{
	Box areaLeft(screenArea.box());
	Point nowPoint(screenArea.point());

	const float drawnStartOffset = (area.point().*point_getter)();
	const float drawnEndOffset = drawnStartOffset+(area.box().*changed_getter)();
	float offset = 0;
	for(shared_ptr<SplitCtx> ctx : this->children()){
		const float size = ctx->size;
		if(offset+size < drawnStartOffset){
			offset += size;
			continue;
		}else if(drawnEndOffset <= offset){
			break;
		}
		Box drawnBox(area.box());
		(drawnBox.*changed_setter)(std::min(size, drawnEndOffset-offset));

		Point drawnPoint(area.point());
		(drawnPoint.*point_setter)(std::min(0.0f, offset-drawnStartOffset));

		Point screenPoint(screenArea.point());
		(screenPoint.*point_setter)(((screenPoint.*point_getter)())+offset-drawnStartOffset);

		ctx->layout->render(canvas, Area(screenPoint, drawnBox), Area(drawnPoint, drawnBox));
		offset += size;
	}
}

Box SplitLayout::onMeasure(const Box& constraint)
{
	const bool changedSpecified = geom::isSpecified((constraint.*changed_getter)());
	const bool fixedSpecified = geom::isSpecified((constraint.*fixed_getter)());
	if(!changedSpecified){
		//いくらでも伸びてよし
		float totalSize = 0;
		float fixedMaxSize = 0;
		for(shared_ptr<SplitCtx> childCtx : this->children()){
			const Box childSize(childCtx->layout->measure(constraint));
			totalSize += this->wrapSize((childSize.*changed_getter)(), childCtx->def);
			fixedMaxSize = std::max(fixedMaxSize, (childSize.*fixed_getter)());
		}
		Box measured;
		(measured.*changed_setter)(totalSize);
		if(fixedSpecified){
			(measured.*fixed_setter)((constraint.*fixed_getter)());
		}else{
			(measured.*fixed_setter)(fixedMaxSize);
		}
		return measured;
	}else{
		//長さは制限される：ふくざつ！！
		const float limitChangedSize = (constraint.*changed_getter)();
		float fixedMaxSize = 0;
		float totalWeight = 0;

		//まず、親から与えられた長さが十分かどうか調べる
		float intendedSizeTotal = 0;
		float nonWeightedSizeTotal = 0;
		for(shared_ptr<SplitCtx> childCtx : this->children()){
			const bool weightSpecified = geom::isSpecified(childCtx->def.weight);
			const Box childSize(childCtx->layout->measure(constraint));
			if(weightSpecified){
				//ウェイトがかかっている場合は最小サイズを。
				intendedSizeTotal += childCtx->def.min;
				childCtx->size = childCtx->def.min;//足りなかった時に使うための仮置き
				totalWeight += childCtx->def.weight;
			}else{
				//そうでない場合は、子レイアウトに言われた通りのサイズを。
				//ただしmin/maxは守る
				const float tempChangedSize = this->wrapSize((childSize.*changed_getter)(), childCtx->def);
				childCtx->size = tempChangedSize;
				intendedSizeTotal += tempChangedSize;
				nonWeightedSizeTotal += tempChangedSize;
			}
			fixedMaxSize = std::max(fixedMaxSize, (childSize.*fixed_getter)());
		}

		if(intendedSizeTotal <= limitChangedSize){
			//十分足りる
			float leftWeight = totalWeight;
			float leftSize = limitChangedSize - nonWeightedSizeTotal;
			for(shared_ptr<SplitCtx> childCtx : this->children()){
				shared_ptr<Layout> child;
				const bool weightSpecified = geom::isSpecified(childCtx->def.weight);
				if(weightSpecified){
					//ウェイトがかかっている
					float size = leftSize * childCtx->def.weight / leftWeight;
					//max/minを考慮しつつサイズを割り当てる
					size = this->wrapSize(size, childCtx->def);
					leftWeight -= childCtx->def.weight;
					leftSize -= size;
					childCtx->size = size;
				}else{
					//上のループですでに設定済みなのでどうでも良かった
				}
			}
		}else{
			//足りない
			const float changedOverrun = intendedSizeTotal-limitChangedSize;
			for(shared_ptr<SplitCtx> childCtx : this->children()){
				//元の割合に応じてサイズを設定
				childCtx->size -= changedOverrun * childCtx->size / intendedSizeTotal;
			}
		}
		Box measured;
		(measured.*changed_setter)(limitChangedSize);
		if(fixedSpecified){
			(measured.*fixed_setter)((constraint.*fixed_getter)());
		}else{
			(measured.*fixed_setter)(fixedMaxSize);
		}
		return measured;
	}
}

void SplitLayout::onLayout(const Box& size)
{
	for(shared_ptr<SplitCtx> ctx : this->children()){
		Box box;
		(box.*changed_setter)(ctx->size);
		(box.*fixed_setter)((size.*fixed_getter)());
		ctx->layout->layout(box);
	}
}

weak_ptr<Layout> SplitLayout::getLayoutByIdImpl(const std::string& id)
{
	for(shared_ptr<SplitCtx> childCtx : this->children()){
		weak_ptr<Layout> res = childCtx->layout->getLayoutById(id);
		if(!res.expired()){
			return res;
		}
	}
	return weak_ptr<Layout>();
}


}}}
