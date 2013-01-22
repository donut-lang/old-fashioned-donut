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

#pragma once
#include "../node/Node.h"

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
