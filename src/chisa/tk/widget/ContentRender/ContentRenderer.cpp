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

#include "../Content/Node.h"
#include "ContentRenderer.h"
#include "RenderCommand.h"

namespace chisa {
namespace tk {
namespace widget {

ContentRenderer::ContentRenderer(gl::Canvas& canvas, const geom::Area& area, float depth)
:canvas_(canvas)
,area_(area)
,depth_(depth)
{

}

void ContentRenderer::walkCommon(Node* node)
{

}

void ContentRenderer::walkTree(TreeNode* node)
{
	this->walkCommon(node);
	this->walkChildren(node);
}
void ContentRenderer::walkImpl(BlockNode* tree)
{
	tree->background()->render(canvas_, area_.point(), this->depth_+this->nowDepth()/10.0);
	this->walkTree(tree);
}
void ContentRenderer::walkImpl(InlineNode* tree)
{
	this->walkTree(tree);
}
void ContentRenderer::walk(Text* text)
{
	for(Text::DataType d : text->objects()){
		geom::Area const intersect(d->area().intersect(this->area_));
		if (!intersect.empty()) {
			d->render(this->canvas_, this->area_.point(), this->depth_+this->nowDepth()/10.0+0.1);
		}else{
			d->onHidden();
		}
	}
}

//-----------------------------------------------------------------------------

void ContentRenderer::walk(Document* doc)
{
	this->walkImpl(doc);
}
void ContentRenderer::walk(Paragraph* para)
{
	this->walkImpl(para);
}
void ContentRenderer::walk(Heading* head)
{
	this->walkImpl(head);
}
void ContentRenderer::walk(Link* link)
{
	this->walkImpl(link);
}
void ContentRenderer::walk(Font* font)
{
	this->walkImpl(font);
}
void ContentRenderer::walk(BreakLine* br)
{
}

}}}
