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

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(HorizontalLayout, SplitLayout)
{

}

HorizontalLayout::~HorizontalLayout()
{
}

string HorizontalLayout::toString()
{
	return util::format("(HorizontalLayout %p)", this);
}

void HorizontalLayout::renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area)
{
	float offset = 0;
	for(shared_ptr<SplitCtx> ctx : this->children()){
		const Area target(area.x()+offset, area.y(), ctx->size, area.height());
		offset += ctx->size;
		//XXX コンパイル通したいだけ
		ctx->layout->render(canvas, target, target);
	}
}

Box HorizontalLayout::onMeasure(const Box& constraint)
{
	/*
	this->resetChildrenLayout();
	float height=constraint.height();
	if(geom::isUnspecified(constraint.width())){
		//横幅未指定なのでどこまでも伸びる
		Box box(geom::Unspecified, constraint.height());
		float width=0.0f;

		for(shared_ptr<SplitCtx> ctx : this->children()){
			const Box result = ctx->layout->measure(box);
			height = SplitLayout::max(height, result.height());
			width += (ctx->size = result.width());
		}
		return tk::Box(width, height);
	}else{
		float width = constraint.width();
		float totalWidth=0;
		float totalWeight = 0;

		for(shared_ptr<SplitCtx> ctx : this->children()) {
			if(geom::isUnspecified(ctx->def.weight)){
				//まずウェイトのかかっていない部分を集計する
				Box c(width, constraint.height());
				Box b = ctx->layout->measure(c);
				float w = b.width();
				height = std::max(height, b.height());
				w = SplitLayout::min(w, ctx->def.max);
				w = SplitLayout::max(w, ctx->def.min);
				totalWidth+=w;
				ctx->size = w;
			}else{
				//ウェイトの掛かった部分はあとで分け合うためにとっておく
				totalWeight += ctx->def.weight;
			}
		}
		if(totalWidth > width){ //長さが足りない
			const float diffWidth = width-totalWidth;
			//FIXME: minを反映したい
			for(shared_ptr<SplitCtx> ctx : this->children()) {
				if(geom::isUnspecified(ctx->def.weight)){
					ctx->size = 0;
				}else{
					ctx->size += diffWidth * ctx->size /totalWidth;
				}
			}
			return Box(width, constraint.height());
		}else{ //固定のを自由にさせても余ったので、ウェイトで分かち合う
			const float diffWidth = width-totalWidth;
			float leftWidth = diffWidth;
			for(shared_ptr<SplitCtx> ctx : this->children()) {
				if(geom::isSpecified(ctx->def.weight)){
					Box c(diffWidth * ctx->def.weight / totalWeight, constraint.height());
					Box b = ctx->layout->measure(c);

					ctx->size = b.width();
					height = SplitLayout::max(height, b.height());
					ctx->size = SplitLayout::min(ctx->size, ctx->def.max);
					ctx->size = SplitLayout::max(ctx->size, ctx->def.min);
					leftWidth -= ctx->size;
				}
			}
			return Box(width-leftWidth, constraint.height());
		}
	}*/
}

void HorizontalLayout::onLayout(const Box& size)
{
	/*
	const Box calc = this->measure(area.box());
	const float scale = area.width()/calc.width();
	float offset = 0;
	for(shared_ptr<SplitCtx> ctx : this->children()){
		if(scale < 1){
			ctx->size *= scale;
		}
		ctx->layout->reshape(Area(area.x()+offset, area.y(), ctx->size, area.height()));
		offset += ctx->size;
	}*/
}

}}}
