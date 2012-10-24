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

#include "../../gl/StringRenderer.h"
#include "ContentMeasurer.h"
#include "Content/Node.h"
#include "../../geom/Area.h"

namespace chisa {
namespace tk {
namespace widget {

ContentMeasurer::ContentMeasurer(float const width) noexcept
:widgetWidth_(width)
,nowSession_(nullptr)
,surface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,cairo_(cairo_create(this->surface_))
{
}

ContentMeasurer::BlockSession::BlockSession(ContentMeasurer& parent, BlockNode* const node)
:node_(node)
,parent_(parent)
,lastSession_(parent_.nowSession_)
,offsetFromTop_(lastSession_->offsetFromTop()+this->node_->margin().offset())
,maxWidth_(geom::min(node->width(), lastSession_->maxWidth() - this->node_->margin().totalWidth()))
,lastDirection_(BlockNode::Direction::None)
,consumedHeight_(0.0f)
,reservedBlockWidth_(0.0f)
,reservedBlockHeight_(0.0f)
,reservedInlineWidth_(0.0f)
,reservedInlineHeight_(0.0f)
{
	parent.nowSession_ = this;
}
ContentMeasurer::BlockSession::~BlockSession() noexcept
{
	this->lastSession_->extendBlock(geom::Box(this->maxWidth_, this->consumedHeight_), node_->direction());
	this->parent_.nowSession_ = this->lastSession_;
}

geom::Area ContentMeasurer::BlockSession::extendBlock(const geom::Box& size, BlockNode::Direction dir)
{
	this->flushInline();
	float const limit = this->calcLeftWidth();
	geom::Area area;
	if(size.width() < limit && this->lastDirection() != BlockNode::Direction::None){
		//隣にくっつける
		if(this->lastDirection() == BlockNode::Direction::Left){
			area = geom::Area(this->offsetFromTop()+geom::Distance(limit - this->reservedBlockWidth() - size.width(), this->consumedHeight()), size);
			this->reservedBlockWidth(this->reservedBlockWidth()+size.width());
			this->reservedBlockHeight(geom::max(this->reservedBlockHeight(), size.height()));
		}else if(this->lastDirection() == BlockNode::Direction::Right){
			area = geom::Area(this->offsetFromTop()+geom::Distance(this->reservedBlockWidth(), this->consumedHeight()), size);
			this->reservedBlockWidth(this->reservedBlockWidth()+size.width());
			this->reservedBlockHeight(geom::max(this->reservedBlockHeight(), size.height()));
		}else{
			throw logging::Exception(__FILE__, __LINE__, "[Bug] Unknwon direction: %d", this->lastDirection());
		}
	}else{
		//下へ
		this->consumedHeight(this->consumedHeight() + this->reservedBlockHeight());
		this->reservedBlockWidth(size.width());
		this->reservedBlockHeight(size.height());
		area = geom::Area(this->offsetFromTop() + geom::Distance(dir == BlockNode::Left ? limit-size.width() : 0, this->consumedHeight()),size);
	}
	this->lastDirection(dir == this->lastDirection() || this->lastDirection() == BlockNode::Direction::None ? dir : BlockNode::Direction::None);
	return area;
}

geom::Area ContentMeasurer::BlockSession::extendInline(const geom::Box& size)
{
}

void ContentMeasurer::BlockSession::flushInline()
{
	if(!(this->reservedInlineHeight() > geom::VerySmall || this->reservedInlineWidth() > geom::VerySmall)){
		return;
	}
	this->consumedHeight_ += geom::max(this->reservedBlockHeight(), this->reservedInlineHeight());
	this->reservedInlineWidth(0.0f);
	this->reservedInlineHeight(0.0f);
	this->reservedBlockWidth(0.0f);
	this->reservedBlockHeight(0.0f);
	this->lastDirection(BlockNode::Direction::None);
}

void ContentMeasurer::BlockSession::flushBlock()
{
	this->flushInline();
	if(!(this->reservedBlockHeight() > geom::VerySmall || this->reservedBlockWidth() > geom::VerySmall)){
		return;
	}
	this->consumedHeight_ += this->reservedBlockHeight();
	this->reservedBlockWidth(0.0f);
	this->reservedBlockHeight(0.0f);
	this->lastDirection(BlockNode::Direction::None);
}

float ContentMeasurer::BlockSession::calcLeftWidth()
{
	return consumedHeight_ <= lastSession_->reservedBlockHeight() ?
				lastSession_->maxWidth()-lastSession_->reservedBlockWidth() : //親ノードの最大値-reservedWidth;
				maxWidth();//nodeが最大値を持ってるならそれ、無いなら親セッションの最大幅
}
void ContentMeasurer::walk(Document* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasurer::walk(Paragraph* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasurer::walk(Heading* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasurer::walk(Link* model)
{
}

geom::Area ContentMeasurer::extendBlock(const geom::Box& size, BlockNode::Direction dir)
{
	return this->nowSession_->extendBlock(size,dir);
}

geom::Area ContentMeasurer::extendInline(const geom::Box& size)
{
	return this->nowSession_->extendInline(size);
}

void ContentMeasurer::walk(Text* model)
{
	float width = this->nowSession_->calcLeftWidth();
	model->clearRenderCommands();
	gl::StringRenderer renderer;
	std::string str = model->text();
	size_t now=0;
	while(now < str.length()){
		gl::StringRenderer::Command cmd = renderer.calcMaximumStringLength(str, width, now);
		if(!cmd){//そもそも１文字すら入らない
			//行送り
			continue;
		}
		now += cmd.str().size();
		//TODO: 文字分のエリアを予約
		//コマンドを追加。本来はポジションとかも含めて入れないと行けないはず〜。
		geom::Area rendered = this->extendInline(cmd.area().box());
		Text::RenderSet set(cmd, rendered);
		std::vector<chisa::gl::StringRenderer::Command>().push_back(cmd);
	}
}

}}}
