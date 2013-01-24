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

#include "ContentUnderlineServant.h"
#include "../Element.h"
#include "../World.h"
#include "../donut/Patron.h"
#include "../widget/ContentWidget.h"
#include "../../doc/node/Node.h"

namespace chisa {
namespace tk {

ContentUnderlineServant::ContentUnderlineServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color, std::string const& docId)
:Servant(angelTarget)
,color_(color)
,docId_(docId)
{
}

Handler<ContentUnderlineServant> ContentUnderlineServant::self() noexcept
{
	return Handler<ContentUnderlineServant>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> ContentUnderlineServant::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->contentUnderlineServantProvider()->newInstance(heap, self()) );
}

void ContentUnderlineServant::renderImpl(gl::Canvas& canvas)
{
	Handler<AngelWidgetTarget> target = this->target()->toWidgetTarget();
	if( unlikely(!target) ) {
		return;
	}
	Handler<ContentWidget> widget ( target->findWidget().cast<ContentWidget>() );
	if( unlikely(!widget) ) {
		return;
	}
	::chisa::doc::TreeNode* node = widget->findTreeNodeById(this->docId_);
	if( unlikely(!node) ) {
		return;
	}
	doc::Text* tex = node->findFirstTextNode();
	std::size_t max = tex->objectCount();
	for(std::size_t i = 0; i < max; ++i){
		doc::Text::DataType type = tex->objectAt(i);
		geom::Point start(type->area().left(), type->area().bottom());
		geom::Point end(type->area().right(), type->area().bottom());
		canvas.drawLine(3, color_, start, end);
	}
}

void ContentUnderlineServant::idleImpl(const float delta_ms)
{
}

geom::Box ContentUnderlineServant::reshapeImpl(const geom::Area& area)
{
	return this->target()->findScreenArea().box();
}

}}

