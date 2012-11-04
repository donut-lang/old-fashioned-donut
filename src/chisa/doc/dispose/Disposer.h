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

#include "../node/NodeWalker.h"
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../geom/Area.h"
#include "TextContext.h"

namespace chisa {
namespace doc {
class BlockSession;

class Disposer: public chisa::doc::NodeWalker {
	DEFINE_MEMBER_REF(private, logging::Logger, log)
public:
	Disposer(logging::Logger& log, Handler<RenderTree> renderTree, const float widgetWidth);
	virtual ~Disposer() noexcept(true) = default;
private:
	Handler<RenderTree> renderTree_;
	Handler<gl::DrawableManager> dmanager_;
	const float widgetWidth_;
	TextContext textContext_;
	BlockSession* nowSession_;
	float nowDepth_;
private:
	void newInline();
	void extendInline(Handler<RenderObject> obj);
	void extendBlock(BlockNode* blockNode);
	template <typename T, typename U> void walkBlock(T* block, U clos);
public:
	BlockSession* nowSession();
	void rewriteSession(BlockSession* session);
public:
	geom::Box start(std::shared_ptr<Document> doc);
	virtual void walk(Document* doc) override;
	virtual void walk(Paragraph* para) override;
	virtual void walk(Heading* head) override;
	virtual void walk(Text* text) override;
	virtual void walk(Link* link) override;
	virtual void walk(Font* font) override;
	virtual void walk(BreakLine* br) override;
};

}}
