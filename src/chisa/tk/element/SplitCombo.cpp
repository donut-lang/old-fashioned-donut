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

#include "SplitCombo.h"
#include "ElementFactory.h"
#include "../../geom/Vector.h"

namespace chisa {
namespace tk {

static const std::string TAG("SplitCombo");
const std::string SplitCombo::AttrName::Weight("split-weight");
const std::string SplitCombo::AttrName::Max("split-max");
const std::string SplitCombo::AttrName::Min("split-min");


CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(SplitCombo, Super)
,splitMode_(Vertical)
,totalSize_(geom::Unspecified)
,changed_getter(nullptr)
,changed_setter(nullptr)
,fixed_getter(nullptr)
,fixed_setter(nullptr)
,point_getter(nullptr)
,point_setter(nullptr)
{
	this->setMode(Vertical);
}

void SplitCombo::setMode(enum SplitMode mode)
{
	this->splitMode(mode);
	changed_getter = mode == Vertical ? (float(geom::Box::*)(void) const)&geom::Box::height : (float(geom::Box::*)(void) const)&geom::Box::width;
	changed_setter = mode == Vertical ? (void(geom::Box::*)(float))&geom::Box::height : (void(geom::Box::*)(float))&geom::Box::width;
	fixed_getter = mode == Vertical ? (float(geom::Box::*)(void) const)&geom::Box::width : (float(geom::Box::*)(void) const)&geom::Box::height;
	fixed_setter = mode == Vertical ? (void(geom::Box::*)(float))&geom::Box::width : (void(geom::Box::*)(float))&geom::Box::height;
	point_getter = mode == Vertical ? (float(geom::Point::*)(void) const)&geom::Point::y : (float(geom::Point::*)(void) const)&geom::Point::x;
	point_setter = mode == Vertical ? (void(geom::Point::*)(float))&geom::Point::y : (void(geom::Point::*)(float))&geom::Point::x;
}

SplitCombo::~SplitCombo() noexcept
{
}

std::string SplitCombo::toString() const
{
	if(this->splitMode() == Horizontal){
		return ::tarte::format("(HorizontalCombo %p)", this);
	}else if(this->splitMode() == Vertical){
		return ::tarte::format("(VerticalCombo %p)", this);
	}else{
		return ::tarte::format("(InvalidSplitCombo %p)", this);
	}
}

void SplitCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* top)
{
	const std::string name(top->Name());
	if(name == "horizontal"){
		this->setMode(Horizontal);
	}else if(name == "vertical"){
		this->setMode(Vertical);
	}else{
		log().e(TAG, "Oops. you might miss-spelled? \"s\"", name.c_str());
		this->setMode(Vertical);
	}
	for(tinyxml2::XMLElement* elem = top->FirstChildElement(); elem; elem=elem->NextSiblingElement()){
		float weight = geom::Unspecified;
		float min = geom::Unspecified;
		float max = geom::Unspecified;
		elem->QueryFloatAttribute(AttrName::Weight.c_str(), &weight);
		elem->QueryFloatAttribute(AttrName::Min.c_str(), &min);
		elem->QueryFloatAttribute(AttrName::Max.c_str(), &max);
		//デフォルト値の設定
		if(geom::isUnspecified(min)){
			min = 0;
		}
		if(geom::isUnspecified(max)){
			max = geom::Unspecified;
		}
		this->addChild(factory->parseTree(self(), elem), SplitComboContext(weight, min, max));
	}
}

void SplitCombo::resetChildren()
{
	for(ContainerType& ctx: this->children()){
		ctx.second.size=geom::Unspecified;
	}
	this->totalSize_ = geom::Unspecified;
}

void SplitCombo::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	const float boxSize = (mask.box().*changed_getter)();
	const float drawnStartOffset = (mask.point().*point_getter)();
	const float drawnEndOffset = drawnStartOffset+boxSize;
	const float screenStart = ((ptInRoot+mask.point()).*point_getter)();
	float offset = 0;
	for(ContainerType& ctx: this->children()){
		const float size = ctx.second.size;
		if(offset+size < drawnStartOffset){
			offset += size;
			continue;
		}else if(drawnEndOffset <= offset){
			break;
		}
		const float drawnStart = std::max(drawnStartOffset - offset, 0.0f);
		const float drawnSize = std::min(size-drawnStart, drawnEndOffset-offset-drawnStart);

		geom::Box drawnBox(mask.box());
		(drawnBox.*changed_setter)(drawnSize);

		geom::Point drawnPoint(mask.point());
		(drawnPoint.*point_setter)(drawnStart);

		geom::Point screenPoint(ptInRoot);
		(screenPoint.*point_setter)(screenStart+offset-drawnStartOffset);

		ctx.first->render(canvas, screenPoint, geom::Area(drawnPoint, drawnBox));
		offset += size;
	}
}

geom::Box SplitCombo::measureImpl(geom::Box const& constraint)
{
	//this->resetChildren();
	const bool changedSpecified = geom::isSpecified((constraint.*changed_getter)());
	const bool fixedSpecified = geom::isSpecified((constraint.*fixed_getter)());
	geom::Box cbox(constraint);
	(cbox.*changed_setter)(geom::Unspecified);
	if(!changedSpecified){
		//いくらでも伸びてよし
		float totalSize = 0;
		float fixedMaxSize = 0;
		for(ContainerType& it : this->children()){
			SplitComboContext& childCtx = it.second;
			const geom::Box childSize(it.first->measure(cbox));
			const float size = this->wrapSize((childSize.*changed_getter)(), childCtx.def);
			if(geom::isSpecified(size)){
				totalSize += size;
				childCtx.size = size;
			}else if(geom::isSpecified(childCtx.def.max)){
				totalSize += childCtx.def.max;
				childCtx.size = childCtx.def.max;
			}else if(geom::isSpecified(childCtx.def.min)){
				totalSize += childCtx.def.min;
				childCtx.size = childCtx.def.min;
			}else{
				childCtx.weight = 1.0f;
				childCtx.size = geom::Unspecified;
			}
			if(geom::isSpecified((childSize.*fixed_getter)())) {
				fixedMaxSize = std::max(fixedMaxSize, (childSize.*fixed_getter)());
			}
		}
		geom::Box measured;
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
		for(ContainerType& it : this->children()){
			SplitComboContext& childCtx = it.second;
			const bool weightSpecified = geom::isSpecified(childCtx.def.weight);
			const geom::Box childSize(it.first->measure(cbox));
			if(weightSpecified){
				//ウェイトがかかっている場合は最小サイズを。
				childCtx.weight = childCtx.def.weight;
				intendedSizeTotal += childCtx.def.min;
				childCtx.size = childCtx.def.min;//足りなかった時に使うための仮置き
				totalWeight += childCtx.weight;
			}else if(geom::isUnspecified((childSize.*changed_getter)())){
				childCtx.weight = 1.0f;
				intendedSizeTotal += childCtx.def.min;
				childCtx.size = childCtx.def.min;//足りなかった時に使うための仮置き
				totalWeight += childCtx.weight;
			}else{
				//そうでない場合は、子レイアウトに言われた通りのサイズを。
				//ただしmin/maxは守る
				const float tempChangedSize = this->wrapSize((childSize.*changed_getter)(), childCtx.def);
				childCtx.size = tempChangedSize;
				intendedSizeTotal += tempChangedSize;
				nonWeightedSizeTotal += tempChangedSize;
			}
			if(geom::isSpecified((childSize.*fixed_getter)())) {
				fixedMaxSize = std::max(fixedMaxSize, (childSize.*fixed_getter)());
			}
		}

		if(intendedSizeTotal <= limitChangedSize){
			//十分足りる
			float leftWeight = totalWeight;
			float leftSize = limitChangedSize - nonWeightedSizeTotal;
			for(ContainerType& it : this->children()){
				SplitComboContext& childCtx = it.second;
				Handler<Element> child;
				const bool weightSpecified = geom::isSpecified(childCtx.weight);
				if(weightSpecified){
					//ウェイトがかかっている
					float size = leftSize * childCtx.weight / leftWeight;
					//max/minを考慮しつつサイズを割り当てる
					size = this->wrapSize(size, childCtx.def);
					leftWeight -= childCtx.weight;
					leftSize -= size;
					childCtx.size = size;
				}else{
					//上のループですでに設定済みなのでどうでも良かった
				}
			}
		}else{
			//足りない
			const float changedOverrun = intendedSizeTotal-limitChangedSize;
			for(ContainerType& it : this->children()){
				SplitComboContext& childCtx = it.second;
				//元の割合に応じてサイズを設定
				childCtx.size -= changedOverrun * childCtx.size / intendedSizeTotal;
			}
		}
		geom::Box measured;
		(measured.*changed_setter)(limitChangedSize);
		if(fixedSpecified){
			(measured.*fixed_setter)((constraint.*fixed_getter)());
		}else{
			(measured.*fixed_setter)(fixedMaxSize);
		}
		return measured;
	}
}

void SplitCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
#ifdef DEBUG
		if(geom::isUnspecified((size.*changed_getter)())) {
			TARTE_EXCEPTION(Exception, "[BUG] Fixed size is unspecified for SplitCombo.");
		}
		if(geom::isUnspecified((size.*fixed_getter)())) {
			TARTE_EXCEPTION(Exception, "[BUG] Fixed size is unspecified for SplitCombo.");
		}
#endif
	float totalUnspecifiedWidth = 0.0f;
	float left = (size.*changed_getter)();
	for(ContainerType& it : this->children()){
		SplitComboContext& ctx = it.second;
		if( geom::isUnspecified(ctx.size) ) {
			totalUnspecifiedWidth+=ctx.weight;
#ifdef DEBUG
			if(geom::isUnspecified(ctx.weight)) {
				TARTE_EXCEPTION(Exception, "[BUG] Weight is unspecified.");
			}
#endif
		}else{
			left -= ctx.size;
		}
	}
	if(totalUnspecifiedWidth > 0.0f) {
		for(ContainerType& it : this->children()){
			SplitComboContext& ctx = it.second;
			if( geom::isUnspecified(ctx.size) ) {
				float const s = left * ctx.weight / totalUnspecifiedWidth;
				left -= s;
				totalUnspecifiedWidth+=ctx.weight;
				ctx.size = s;
			}
		}
	}
	geom::Box offset(geom::ZERO);
	for(ContainerType& it : this->children()){
		SplitComboContext& ctx = it.second;
		geom::Box box;
		(box.*changed_setter)(ctx.size);
		(box.*fixed_setter)((size.*fixed_getter)());
		(offset.*changed_setter)(ctx.size+(box.*changed_getter)());
#ifdef DEBUG
		if(geom::isUnspecified(ctx.size)) {
			TARTE_EXCEPTION(Exception, "[BUG] Split size is unspecified.");
		}
#endif
		it.first->layout(offset, box);
	}
}

}}
