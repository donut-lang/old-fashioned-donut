/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "ContentUnderlineServant.hpp"
#include "../Element.hpp"
#include "../World.hpp"
#include "../donut/Patron.hpp"
#include "../widget/ContentWidget.hpp"
#include "../../doc/node/Node.hpp"

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
