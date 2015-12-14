/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <memory>
#include "Decl.hpp"

namespace chisa {
namespace doc {

class NodeWalker {
private:
	int nodeDepth_;
protected:
	int nowDepth() const noexcept { return this->nodeDepth_; };
public:
	NodeWalker() noexcept;
	virtual ~NodeWalker() noexcept (true) = default;
	void start(std::shared_ptr<Document> model);
	void walkChildren(TreeNode* children);
	virtual void walk(Document* doc) = 0;
	virtual void walk(Paragraph* para) = 0;
	virtual void walk(Heading* head) = 0;
	virtual void walk(Text* text) = 0;
	virtual void walk(Link* link) = 0;
	virtual void walk(Font* font) = 0;
	virtual void walk(BreakLine* br) = 0;
};

}}
