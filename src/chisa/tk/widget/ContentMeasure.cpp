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
#include "ContentWidget.h"
#include "Content/Node.h"
#include "../../geom/Area.h"

namespace chisa {
namespace tk {
namespace widget {

ContentMeasure::ContentMeasure(float const width) noexcept
:widgetWidth_(width)
,direction_(Right)
,nowSession_(nullptr)
,surface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,cairo_(cairo_create(this->surface_))
{
}

ContentMeasure::BlockSession::BlockSession(ContentMeasure& parent, BlockNode* const node)
:node_(node)
,lastSession_(parent.nowSession_)
,parent_(parent)
,maxWidth_(geom::min(node->width(), lastSession_->maxWidth_))
,consumedHeight_(0.0f)
,reservedWidth_(0.0f)
,reservedHeight_(0.0f)
{
	parent.nowSession_ = this;
}
ContentMeasure::BlockSession::~BlockSession() noexcept
{
	this->parent_.nowSession_ = this->lastSession_;
}

float ContentMeasure::BlockSession::calcLeftWidth()
{
	return consumedHeight_ <= lastSession_->reservedHeight_ ?
				lastSession_->maxWidth_-lastSession_->reservedWidth_ : //親ノードの最大値-reservedWidth;
				maxWidth_;//nodeが最大値を持ってるならそれ、無いなら親セッションの最大幅
}
void ContentMeasure::walk(Document* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasure::walk(Paragraph* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasure::walk(Heading* model)
{
	BlockSession bs(*this, model);
}

void ContentMeasure::walk(Link* model)
{
}

void ContentMeasure::walk(Text* model)
{
	float width = this->nowSession_->calcLeftWidth();
	gl::StringRenderer renderer;
	std::string str = model->text();
	size_t now=0;
	size_t end=1;
	while(now < end){
		geom::Area area;
		size_t rendererd;
		std::tuple<geom::Area, size_t, size_t> r = renderer.calcMaximumStringLength(str, width, now);
		std::tie(area, rendererd, end) = r;
		if(rendererd == now){//そもそも１文字すら入らない
			//行送り
			continue;
		}
		now = rendererd;
	}
}

}}}
