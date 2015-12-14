/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../node/Node.hpp"

namespace chisa {
namespace doc {
class Disposer;

class BlockSession {
	DISABLE_COPY_AND_ASSIGN(BlockSession);
	STACK_OBJECT(BlockSession);
private:
	Disposer* const dispoer_;
	BlockSession* const parentSession_;
	BlockNode* const node_;
private:
	BlockNode::Direction dir_;
private:
	geom::Box const limitSize_;
private:
	float consumedHeight_;
	float consumedWidth_;
private:
	float inlineConsumedHeight_;
	float inlinePosX_;
	float inlineHeight_;
	std::vector<Handler<RenderObject> > inlineObjects_;
private:
	float blockConsumedHeight_;
	float blockPosX_;
public:
	BlockSession(Disposer* disposer, BlockNode* node);
	BlockSession(Disposer* disposer, geom::Box const& limitSize);
	~BlockSession() noexcept;
private:
	void newBlockLine();
	void disposeInlineObject();
public:
	void newInline();
	void extendInline(Handler<RenderObject> obj);
	void extendBlock(BlockNode* blockNode);
	float calcBlockLimit() const;
};

}}
