/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "BlockSession.h"
#include "Disposer.h"

namespace chisa {
namespace doc {

BlockSession::BlockSession(Disposer* disposer, BlockNode* node)
:dispoer_(disposer)
,parentSession_(disposer->nowSession() )
,node_(node)
,dir_(BlockNode::Direction::Unspecified)
,limitSize_( parentSession_->calcBlockLimit() - node->margin().totalWidth(), parentSession_->limitSize_.height() )
,consumedHeight_(0.0f)
,consumedWidth_(0.0f)
,inlineConsumedHeight_(0.0f)
,inlinePosX_(0.0f)
,inlineHeight_(0.0f)
,blockConsumedHeight_(0.0f)
,blockPosX_(0.0f)
{
	disposer->rewriteSession(this);
}

BlockSession::BlockSession(Disposer* disposer, geom::Box const& limitSize)
:dispoer_(disposer)
,parentSession_( nullptr )
,node_(nullptr)
,dir_(BlockNode::Direction::Unspecified)
,limitSize_( limitSize )
,consumedHeight_(0.0f)
,consumedWidth_(0.0f)
,inlineConsumedHeight_(0.0f)
,inlinePosX_(0.0f)
,inlineHeight_(0.0f)
,blockConsumedHeight_(0.0f)
,blockPosX_(0.0f)
{
	disposer->rewriteSession(this);
}
BlockSession::~BlockSession() noexcept
{
	this->newInline();
	this->newBlockLine();
	if(this->node_){
		this->node_->areaInBlock(geom::Area(0,0,this->consumedWidth_, node_->isRoot() ? std::max(this->consumedHeight_, limitSize_.height()) : this->consumedHeight_));
		if(this->parentSession_){
			this->parentSession_->extendBlock(this->node_);
		}
	}
	dispoer_->rewriteSession(this->parentSession_);
}
void BlockSession::disposeInlineObject()
{
	for(Handler<RenderObject> obj : this->inlineObjects_){
		//Y位置だけは最後にここで計算する。他はすでに計算済み
		obj->area().y(obj->area().y() + (this->inlineHeight_-obj->area().height())/2);
	}
	this->inlineObjects_.clear();
}
void BlockSession::newBlockLine()
{
	float consumedHeight = std::max(this->inlineConsumedHeight_, this->blockConsumedHeight_);
	this->consumedHeight_ += consumedHeight;
	this->consumedWidth_ = std::max( this->consumedWidth_, this->blockPosX_ + this->inlinePosX_);
	//
	this->blockConsumedHeight_ = 0.0f;
	this->blockPosX_ = 0.0f;
	//
	this->disposeInlineObject();
	this->inlineConsumedHeight_ = 0.0f;
	this->inlineHeight_ = 0.0f;
	this->inlinePosX_ = 0.0f;
	//
	this->dir_ = BlockNode::Direction::Unspecified;
}
void BlockSession::newInline()
{
	this->disposeInlineObject();
	this->inlineConsumedHeight_ += this->inlineHeight_;
	this->consumedWidth_ = std::max( this->consumedWidth_, this->blockPosX_ + this->inlinePosX_);
	if(this->inlineConsumedHeight_ >= this->blockConsumedHeight_){
		this->newBlockLine();
		this->inlineConsumedHeight_ = 0.0f;
		this->inlineHeight_ = 0.0f;
		this->inlinePosX_ = 0.0f;
	}else{
		this->inlineHeight_ = 0.0f;
		this->inlinePosX_ = 0.0f;
	}
}

void BlockSession::extendInline(Handler<RenderObject> obj)
{
	if(obj->width() > this->calcBlockLimit() - this->inlinePosX_ ){
		this->newInline();
	}
	if(this->dir_ == BlockNode::Direction::Left){
		obj->area(geom::Area(this->inlinePosX_, this->inlineConsumedHeight_+this->consumedHeight_, obj->width(), obj->height()));
		this->inlinePosX_ += obj->width();
		this->inlineHeight_ = std::max( this->inlineHeight_, obj->height() );
		this->inlineObjects_.push_back(obj);
	}else{
		obj->area(geom::Area(this->blockPosX_+this->inlinePosX_, this->inlineConsumedHeight_+this->consumedHeight_, obj->width(), obj->height()));
		this->inlinePosX_ += obj->width();
		this->inlineHeight_ = std::max( this->inlineHeight_, obj->height() );
		this->inlineObjects_.push_back(obj);
	}
}

float BlockSession::calcBlockLimit() const
{
	return this->limitSize_.width() - this->blockPosX_;
}

void BlockSession::extendBlock(BlockNode* blockNode)
{
	if(this->inlineObjects_.size() > 0){
		this->newInline();
	}
	//ここまでで、インライン要素が一切挿入されていないことが保証される
	geom::Box const size = blockNode->areaInBlock().box() + blockNode->margin().totalSpace();
	if(size.width() > this->calcBlockLimit()){
		this->newBlockLine();
	}
	if(this->dir_ == BlockNode::Direction::Unspecified){
		this->dir_ = blockNode->direction();
	}
	switch (this->dir_) {
	case BlockNode::Direction::None:
		this->newBlockLine();
		blockNode->areaInBlock(geom::Area(0, this->consumedHeight_, limitSize_.width(), size.height()));
		this->consumedHeight_ += size.height();
		break;
	case BlockNode::Direction::Right:
		if(blockNode->direction() != BlockNode::Direction::Right){
			this->newBlockLine();
			this->dir_ = blockNode->direction();
		}
		blockNode->areaInBlock(geom::Area(this->blockPosX_, this->consumedHeight_, size.width(), size.height()));
		this->blockConsumedHeight_ = std::max(this->blockConsumedHeight_, size.height());
		this->blockPosX_ += size.width();
		break;
	case BlockNode::Direction::Left:
		if(blockNode->direction() != BlockNode::Direction::Left){
			this->newBlockLine();
			this->dir_ = blockNode->direction();
		}
		blockNode->areaInBlock(geom::Area(limitSize_.width() - this->blockPosX_ - size.width(), this->consumedHeight_, size.width(), size.height()));
		this->blockConsumedHeight_ = std::max(this->blockConsumedHeight_, size.height());
		this->blockPosX_ += size.width();
		break;
	default:
		CINAMO_EXCEPTION(Exception, "[BUG] Unknown direction: %d", this->dir_);
	}
}


}}
