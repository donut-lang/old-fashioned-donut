/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "ElementServant.hpp"
#include "../Element.hpp"
#include "../World.hpp"
#include "../donut/Patron.hpp"
#include "../../gl/DecoUtil.hpp"

namespace chisa {
namespace tk {

ElementServant::ElementServant(Handler<AngelTarget> const& angelTarget, Handler<Element> const& elem)
:Servant(angelTarget)
,element_(elem)
,anim_(0)
,animEnabled_(true)
{

}

Handler<ElementServant> ElementServant::self() noexcept
{
	return Handler<ElementServant>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> ElementServant::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->elementServantProvider()->newInstance(heap, self()) );
}

void ElementServant::renderImpl(gl::Canvas& canvas)
{
	geom::Box box = this->element_->size();
	geom::Area area = this->target()->findScreenArea();

	if(area.empty()){
		return;
	}

	float offsetX = 0;
	float lineOffsetX = 0;
	float midX;
	if( area.right() + box.width() < renderArea().right() ) { //右側に描けるよ！
		lineOffsetX = offsetX = area.right();
		midX = box.width();
	}else if( area.left() - box.width() > 0 ){ //左なら開いてるよ！
		offsetX = area.left() - box.width();
		lineOffsetX = area.left();
		midX=0;
	}else{
		lineOffsetX = area.left();
		offsetX = 0;
		midX=0;
	}

	float offsetY = 0;
	float const midY = (area.top() + area.bottom())/2;
	if( area.bottom() + box.height() < renderArea().bottom() ) {
		offsetY = area.bottom();
	}else if( area.top() - box.height() > 0 ) {
		offsetY = area.top() - box.height();
	}else{
		offsetY = 0;
	}

	float const cycleY = animEnabled_ ? std::sin( anim_/1000.0f ) * box.height() * 0.1f : 0;
	float const cycleX = animEnabled_ ? std::sin( anim_/1740.0f ) * box.width() * 0.1f : 0;

	geom::Point pt(offsetX+cycleX, offsetY+cycleY);
	geom::Box leftSize( renderArea().width()-offsetX, renderArea().height()-offsetY );
	geom::Point lpt(offsetX+cycleX+midX, offsetY+cycleY);

	gl::renderCubicLine(canvas, 3, gl::Color(1.0f,.0f,.0f,.3f), geom::Point(lineOffsetX, midY), lpt);
	geom::Box maskSize(geom::min(box, leftSize));
	if(maskSize.width() > 0 && maskSize.height() > 0) {
		this->element()->render(canvas, pt, geom::Area(geom::ZERO, maskSize));
	}
}

void ElementServant::idleImpl(const float delta_ms)
{
	this->anim_ += delta_ms;
	this->element_->idle(delta_ms);
}

geom::Box ElementServant::reshapeImpl(const geom::Area& area)
{
	geom::Box requested(element()->measure(geom::Box()));
	geom::Box _size(
			geom::isUnspecified(requested.width()) ? area.box().width()/4 : requested.width(),
			geom::isUnspecified(requested.height()) ? area.box().height()/4 : requested.height()
			);
	element()->layout(geom::ZERO, _size);
	return _size;
}

Handler<Element> ElementServant::findElementById(const std::string& id)
{
	return this->element()->findElementById(id);
}

void ElementServant::onShownImpl()
{
	element()->onShown();
}

void ElementServant::onHiddenImpl()
{
	element()->onHidden();
}

Handler<Element> ElementServant::findElementByPoint(const geom::Point& screenVector)
{
	return this->element()->findElementByPoint(screenVector);
}

}}
