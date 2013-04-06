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

#include "Widget.h"
#include "element/WidgetElement.h"
#include "World.h"

namespace chisa {
namespace tk {

const static std::string TAG("Widget");

Widget::Widget(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element)
:log_(log)
,world_(world)
{
}

void Widget::onShown()
{
	this->onShownImpl();
}

void Widget::onHidden()
{
	this->onHiddenImpl();
}
void Widget::render(gl::Canvas& cv, geom::Area const& area)
{
	this->renderImpl(cv, area);
	this->lastDrawnArea_ = area;
}
void Widget::idle(const float delta_ms)
{
	this->idleImpl(delta_ms);
}
void Widget::reshape(geom::Box const& areaSize)
{
	if(this->log().t()){
		this->log().t(TAG, "Reshape \"%s\" area: %s", this->toString().c_str(), areaSize.toString().c_str());
	}
	this->reshapeImpl(areaSize);
	this->size_ = areaSize;
}
geom::Box Widget::measure(geom::Box const& constraintSize)
{
	if(this->log().t()){
		this->log().t(TAG, "Measureing \"%s\" constraint: %s", this->toString().c_str(), constraintSize.toString().c_str());
	}
	return this->measureImpl(constraintSize);
}


geom::Area Widget::findTarget(std::string const& target)
{
	return this->findTargetImpl(target);
}
geom::Area Widget::findTargetInRoot(std::string const& target)
{
	Handler<WidgetElement> const elm = this->wrapper_.lock();
	if(!elm){
		return geom::Area();
	}
	return elm->calcAreaInRoot( findTarget(target) );
}
geom::Area Widget::findTargetInElement(std::string const& target)
{
	Handler<WidgetElement> const elm = this->wrapper_.lock();
	if(!elm){
		return geom::Area();
	}
	return elm->calcAreaInElement( findTarget(target) );
}

void Widget::showPoint(geom::Point const& pt)
{
	Handler<WidgetElement> const elm = this->wrapper_.lock();
	if( likely(elm) ){
		elm->showPoint( elm->calcPtInElement( pt ) );
	}
}

void Widget::requestRelayout()
{
	if(Handler<WidgetElement> elm = this->wrapper_.lock()){
		elm->requestRelayout();
	}
}

std::string Widget::toString() const
{
	return ::cinamo::format("(Widget: %p)", this);
}

void Widget::notifyRelayoutFinished()
{
	this->notifyRelayoutFinishedImpl();
}

Handler<WidgetObject> Widget::donutObject()
{
	if(!this->donutObject_.expired()){
		return this->donutObject_.lock();
	}
	Handler<World> world = this->world().lock();
	if( unlikely(!world) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	Handler< ::donut::Donut> donut( world->donut() );
	WidgetFactory* factory = world->widgetFactory();
	Handler<WidgetProvider> provider(factory->getProviderOf(this));
	if( unlikely(!provider) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Provider is not found for \"%s\"", toString().c_str());
	}
	Handler< ::donut::Heap> const& heap = donut->heap();
	Handler<WidgetObject> eobj ( provider->newInstance(heap, self()) );
	this->donutObject_ = eobj;
	return eobj;
}

void Widget::notifyViewRefreshed()
{
	if(Handler<WidgetElement> elm = this->wrapper_.lock()){
		elm->notifyViewRefreshed();
	}
}

}}
