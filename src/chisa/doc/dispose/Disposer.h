/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>

#include "../node/NodeWalker.h"
#include "../../geom/Area.h"
#include "TextContext.h"

namespace chisa {
namespace doc {
class BlockSession;

class Disposer: public chisa::doc::NodeWalker {
	DEFINE_MEMBER_REF(private, Logger, log)
public:
	Disposer(Logger& log, Handler<RenderTree> renderTree, geom::Box const& constraint);
	virtual ~Disposer() noexcept(true) = default;
private:
	Handler<RenderTree> renderTree_;
	Handler<gl::DrawableManager> dmanager_;
	geom::Box const widgetSize_;
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
