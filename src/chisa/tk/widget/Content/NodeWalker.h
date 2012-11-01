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
#include <memory>
#include "Decl.h"

namespace chisa {
namespace tk {
namespace widget {

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
	virtual void walk(Font* br) = 0;
	virtual void walk(BreakLine* br) = 0;
};

}}}
