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
{
}

ContentMeasurer::BlockSession::BlockSession(ContentMeasurer& parent)
:node_()
,parent_(parent)
,lastSession_(nullptr)
,offsetFromTop_(0.0f,0.0f)
,maxWidth_(parent.widgetWidth_)
,lastDirection_(BlockNode::Direction::None)
,consumedWidth_(0.0f)
,consumedHeight_(0.0f)
,reservedBlockWidth_(0.0f)
,reservedBlockHeight_(0.0f)
,reservedInlineWidth_(0.0f)
,reservedInlineHeight_(0.0f)
,inlineHeight_(0.0f)
,inlineRendered_(false)
{
	parent.nowSession_ = this;
}
ContentMeasurer::BlockSession::BlockSession(ContentMeasurer& parent, BlockNode* const node)
:node_(node)
,parent_(parent)
,lastSession_(parent_.nowSession_)
,offsetFromTop_(lastSession_->offsetFromTop()+this->node_->margin().offset())
,maxWidth_(geom::min(node->width(), lastSession_->maxWidth() - this->node_->margin().totalWidth()))
,lastDirection_(BlockNode::Direction::None)
,consumedWidth_(0.0f)
,consumedHeight_(0.0f)
,reservedBlockWidth_(0.0f)
,reservedBlockHeight_(0.0f)
,reservedInlineWidth_(0.0f)
,reservedInlineHeight_(0.0f)
,inlineHeight_(0.0f)
,inlineRendered_(false)
{
	parent.nowSession_ = this;
}
ContentMeasurer::BlockSession::~BlockSession() noexcept
{
	this->flushBlock();
	if(this->lastSession_){
		this->flushBlock();
		this->lastSession_->extendBlock(geom::Box(this->consumedWidth(), this->consumedHeight()), node_->direction());
	}
	this->parent_.nowSession_ = this->lastSession_;
}

geom::Box ContentMeasurer::start(std::shared_ptr<Document> doc)
{
	BlockSession bs(*this);
	this->NodeWalker::start(doc);
	return geom::Box(bs.reservedBlockWidth(), bs.reservedBlockHeight());
}

geom::Area ContentMeasurer::BlockSession::extendBlock(const geom::Box& size, BlockNode::Direction dir)
{
	float const limit = this->calcLeftWidth();
	geom::Area area;
	if(!this->inlineRendered_ && size.width() < limit && this->lastDirection() != BlockNode::Direction::None){
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
		this->flushBlock();
		this->reservedBlockWidth(size.width());
		this->reservedBlockHeight(size.height());
		area = geom::Area(this->offsetFromTop() + geom::Distance(dir == BlockNode::Left ? limit-size.width() : 0, this->consumedHeight()),size);
	}
	this->lastDirection(dir == this->lastDirection() || this->lastDirection() == BlockNode::Direction::None ? dir : BlockNode::Direction::None);
	this->inlineRendered_ = false;
	return area;
}

void ContentMeasurer::BlockSession::nextLine()
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
		this->nextLine();
	}
	if(this->lastDirection() == BlockNode::Direction::Left){
		area = geom::Area(this->offsetFromTop() + geom::Distance(this->reservedInlineWidth(), this->reservedInlineHeight()),size);
		this->reservedInlineWidth( this->reservedInlineWidth() + size.width() );
		this->inlineHeight( geom::max(this->inlineHeight(), size.height()) );
	}else{
		area = geom::Area(this->offsetFromTop() + geom::Distance(this->reservedBlockWidth()+this->reservedInlineWidth(), this->reservedInlineHeight()),size);
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
	this->consumedHeight(this->consumedHeight() + geom::max(this->reservedBlockHeight(), this->reservedInlineHeight()));
	this->consumedWidth(geom::max(this->reservedBlockWidth() + this->reservedInlineWidth(), this->consumedWidth()));
	this->reservedBlockWidth(0.0f);
	this->reservedBlockHeight(0.0f);
	this->reservedInlineWidth(0.0f);
	this->reservedInlineHeight(0.0f);
	this->lastDirection(BlockNode::Direction::None);
}

float ContentMeasurer::BlockSession::calcLeftWidth()
{
	return lastSession_ && this->consumedHeight() <= lastSession_->reservedBlockHeight() ?
				lastSession_->calcLeftWidth() - this->reservedBlockWidth() - this->reservedInlineWidth() : //親ノードの最大値-reservedWidth;
				this->maxWidth() - this->reservedBlockWidth() - this->reservedInlineWidth();//nodeが最大値を持ってるならそれ、無いなら親セッションの最大幅
}

geom::Area ContentMeasurer::extendBlock(const geom::Box& size, BlockNode::Direction dir)
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
	this->nowSession_->nextLine();
}


void ContentMeasurer::walk(Document* model)
{
	BlockSession bs(*this, model);
	this->walkChildren(model);
}

void ContentMeasurer::walk(Paragraph* model)
{
	BlockSession bs(*this, model);
	this->walkChildren(model);
}

void ContentMeasurer::walk(Heading* model)
{
	BlockSession bs(*this, model);
	this->walkChildren(model);
}

void ContentMeasurer::walk(Link* model)
{
	this->walkChildren(model);
}

void ContentMeasurer::walk(Text* model)
{
	model->clearRenderCommands();
	gl::StringRenderer renderer;
	std::vector<std::string> lines;
	util::splitLine(model->text(), lines);
	for(const std::string& str : lines){
		size_t now=0;
		while(now < str.length()){
			gl::StringRenderer::Command cmd = renderer.calcMaximumStringLength(str, this->calcLeftWidth(), now);
			if(!cmd){//そもそも１文字すら入らない
				this->nextLine();
				continue;
			}
			now += cmd.str().size();
			//文字分のエリアを確保し、その位置とレンダリングコマンドを記録
			geom::Area rendered = this->extendInline(cmd.size());
			Text::RenderSet set(cmd, rendered);
			model->renderCommands().push_back(set);
		}
		this->nextLine();
	}
}

}}}
