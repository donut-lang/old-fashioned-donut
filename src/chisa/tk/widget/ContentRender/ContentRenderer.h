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

#include "../Content/NodeWalker.h"

namespace chisa {
namespace tk {
namespace widget {

class ContentRenderer: public chisa::tk::widget::NodeWalker {
private:
	gl::Canvas& canvas_;
	geom::Area area_;
	float depth_;
public:
	ContentRenderer(gl::Canvas& canvas, const geom::Area& area, float depth=0.0f);
	virtual ~ContentRenderer() noexcept = default;
private:
	void walkCommon(Node* node);
	void walkTree(TreeNode* node);
	void walkImpl(BlockNode* tree);
	void walkImpl(InlineNode* tree);
public:
	virtual void walk(Document* doc) override;
	virtual void walk(Paragraph* para) override;
	virtual void walk(Heading* head) override;
	virtual void walk(Text* text) override;
	virtual void walk(Link* link) override;
	virtual void walk(Font* br) override;
	virtual void walk(BreakLine* br) override;
};

}}}
