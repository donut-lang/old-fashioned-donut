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

#include "ContentMeasurer.h"
#include "../Content/Node.h"
#include "../../../gl/StringRenderer.h"
#include "../../../geom/Area.h"

namespace chisa {
namespace tk {
namespace widget {

ContentMeasurer::BlockSession::BlockSession(ContentMeasurer& parent, geom::Area& area)
:node_()
,parent_(parent)
,lastSession_(nullptr)
,blockDirection_(BlockNode::Direction::None)
,offsetFromTop_(0.0f,0.0f)
,maxWidth_(parent.widgetWidth_)
,consumedWidth_(0.0f)
,consumedHeight_(0.0f)
,reservedBlockWidth_(0.0f)
,reservedBlockHeight_(0.0f)
,reservedInlineWidth_(0.0f)
,reservedInlineHeight_(0.0f)
,inlineHeight_(0.0f)
,inlineRendered_(false)
,area_(area)
{
	parent.nowSession_ = this;
}
ContentMeasurer::BlockSession::BlockSession(ContentMeasurer& parent, geom::Area& area, BlockNode* const node)
:node_(node)
,parent_(parent)
,lastSession_(parent_.nowSession_)
,blockDirection_(BlockNode::Direction::None)
,offsetFromTop_(geom::ZERO)
,maxWidth_(geom::min(node->width(), lastSession_->maxWidth() - this->node_->margin().totalWidth()))
,consumedWidth_(0.0f)
,consumedHeight_(0.0f)
,reservedBlockWidth_(0.0f)
,reservedBlockHeight_(0.0f)
,reservedInlineWidth_(0.0f)
,reservedInlineHeight_(0.0f)
,inlineHeight_(0.0f)
,inlineRendered_(false)
,area_(area)
{
	this->lastSession_->flushBlock();
	//XXX: よく考えたらブロックの位置は最後に決まるんだった
	this->offsetFromTop(
			this->lastSession_->offsetFromTop()+
			geom::Distance(0,this->lastSession_->consumedHeight())+
			this->node_->margin().offset());
	parent.nowSession_ = this;
}
ContentMeasurer::BlockSession::~BlockSession() noexcept
{
	this->endLine();
	this->flushBlock();
	if(this->lastSession_){
		this->area_ = this->lastSession_->extendBlock(geom::Box(this->consumedWidth(), this->consumedHeight()), node_->direction());
	}else{
		this->area_ = geom::Area(geom::ZERO, geom::Box(this->consumedWidth(), this->consumedHeight()));
	}
	this->parent_.nowSession_ = this->lastSession_;
}

geom::Area ContentMeasurer::BlockSession::extendBlock(const geom::Box& size, BlockNode::Direction const dir)
{
	float const limit = this->calcLeftWidth();
	geom::Area area;
	float const width = dir == BlockNode::Direction::None ? limit : size.width();
	bool const needFlush = dir != this->blockDirection_;
	if(!needFlush && (!this->inlineRendered_) && size.width() < limit){
		//インライン要素が挿入されておらず、かつ幅がある場合
		switch(dir){
		case BlockNode::Direction::None:
			area = geom::Area(this->offsetFromTop()+geom::Distance(this->reservedBlockWidth(), this->consumedHeight()), size);
			this->reservedBlockWidth(this->reservedBlockWidth()+width);
			this->reservedBlockHeight(geom::max(this->reservedBlockHeight(), size.height()));
			this->flushBlock();
			break;
		case BlockNode::Direction::Right:
			area = geom::Area(this->offsetFromTop()+geom::Distance(this->reservedBlockWidth(), this->consumedHeight()), size);
			this->reservedBlockWidth(this->reservedBlockWidth()+width);
			this->reservedBlockHeight(geom::max(this->reservedBlockHeight(), size.height()));
			break;
		case BlockNode::Direction::Left:
			area = geom::Area(this->offsetFromTop()+geom::Distance(limit - this->reservedBlockWidth() - width, this->consumedHeight()), size);
			this->reservedBlockWidth(this->reservedBlockWidth()+width);
			this->reservedBlockHeight(geom::max(this->reservedBlockHeight(), size.height()));
			break;
		default:
			throw logging::Exception(__FILE__, __LINE__, "[Bug] Unknwon direction: %d", dir);
			break;
		}
	}else{//余裕が無いので次の行
		//下へ
		this->flushBlock();
		area = geom::Area(this->offsetFromTop() + geom::Distance(dir == BlockNode::Left ? limit-width : 0, this->consumedHeight()),size);
		this->reservedBlockWidth(width);
		this->reservedBlockHeight(size.height());
		if(BlockNode::Direction::None == dir){
			this->flushBlock();
		}
		this->blockDirection_ = dir;
	}
	this->inlineRendered_ = false;
	return area;
}

void ContentMeasurer::BlockSession::endLine()
{
	this->reservedInlineHeight( this->reservedInlineHeight() + this->inlineHeight() );
	this->consumedWidth( geom::max(this->consumedWidth(), this->reservedBlockWidth() + this->reservedInlineWidth()) );
	this->reservedInlineWidth(0.0f);
	this->inlineHeight(0.0f);
	if(this->reservedInlineHeight() <= this->reservedBlockHeight()){
		this->flushBlock();
	}
}

geom::Area ContentMeasurer::BlockSession::extendInline(const geom::Box& size)
{
	geom::Area area;
	this->inlineRendered_ = true;
	if(size.width() > this->calcLeftWidth()){
		this->endLine();
	}
	if(this->blockDirection_ == BlockNode::Direction::Left){
		area = geom::Area(this->offsetFromTop() + geom::Distance(this->reservedInlineWidth(), this->consumedHeight()+this->reservedInlineHeight()),size);
		this->reservedInlineWidth( this->reservedInlineWidth() + size.width() );
		this->inlineHeight( geom::max(this->inlineHeight(), size.height()) );
	}else{
		area = geom::Area(this->offsetFromTop() + geom::Distance(this->reservedBlockWidth()+this->reservedInlineWidth(), this->consumedHeight()+this->reservedInlineHeight()),size);
		this->reservedInlineWidth( this->reservedInlineWidth() + size.width() );
		this->inlineHeight( geom::max(this->inlineHeight(), size.height()) );
	}
	return area;
}


void ContentMeasurer::BlockSession::flushBlock()
{
	if(this->reservedBlockHeight() < geom::VerySmall && this->reservedBlockWidth() < geom::VerySmall &&
			this->reservedInlineHeight() < geom::VerySmall && this->reservedInlineWidth() < geom::VerySmall){
		return;
	}
	//XXX: この辺、うまくまとめましょう。
	this->reservedInlineHeight( this->reservedInlineHeight() + this->inlineHeight() );
	this->consumedWidth( geom::max(this->consumedWidth(), this->reservedBlockWidth() + this->reservedInlineWidth()) );
	this->reservedInlineWidth(0.0f);
	this->inlineHeight(0.0f);

	this->consumedHeight(this->consumedHeight() + geom::max(this->reservedBlockHeight(), this->reservedInlineHeight()));
	this->consumedWidth(geom::max(this->reservedBlockWidth() + this->reservedInlineWidth(), this->consumedWidth()));
	this->reservedBlockWidth(0.0f);
	this->reservedBlockHeight(0.0f);
	this->reservedInlineWidth(0.0f);
	this->reservedInlineHeight(0.0f);
	this->blockDirection_ = BlockNode::Direction::None;
}

float ContentMeasurer::BlockSession::calcLeftWidth()
{
	return lastSession_ && this->consumedHeight() <= lastSession_->reservedBlockHeight() ?
				lastSession_->calcLeftWidth() - this->reservedBlockWidth() - this->reservedInlineWidth() : //親ノードの最大値-reservedWidth;
				this->maxWidth() - this->reservedBlockWidth() - this->reservedInlineWidth();//nodeが最大値を持ってるならそれ、無いなら親セッションの最大幅
}

geom::Area ContentMeasurer::extendBlock(const geom::Box& size, BlockNode::Direction const dir)
{
	return this->nowSession_->extendBlock(size,dir);
}

geom::Area ContentMeasurer::extendInline(const geom::Box& size)
{
	return this->nowSession_->extendInline(size);
}

void ContentMeasurer::flushBlock()
{
	this->nowSession_->flushBlock();
}

float ContentMeasurer::calcLeftWidth()
{
	return this->nowSession_->calcLeftWidth();
}

void ContentMeasurer::nextLine()
{
	this->nowSession_->endLine();
}

}}}
