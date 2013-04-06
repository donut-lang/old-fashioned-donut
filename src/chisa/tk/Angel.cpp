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

#include "Angel.h"
#include "Heaven.h"
#include "World.h"
#include "../geom/Area.h"
#include "../gl/Color.h"
#include "heaven/AngelElementTarget.h"
#include "heaven/AngelWidgetTarget.h"
#include "heaven/HaloServant.h"
#include "heaven/ElementServant.h"
#include "heaven/ContentUnderlineServant.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {


Angel::Angel(Handler<Heaven> heaven)
:world_(heaven->world())
,heaven_(heaven)
{
}

Handler<World> Angel::world() const {
	return this->world_.lock();
}
Handler<Heaven> Angel::heaven() const {
	return this->heaven_.lock();
}

void Angel::render(gl::Canvas& canvas)
{
	this->renderImpl(canvas);
	for(Handler<AngelTarget> const& t : this->targets_) {
		t->render(canvas);
	}
}

void Angel::idle(const float delta_ms)
{
	for(Handler<AngelTarget> const& t : this->targets_) {
		t->idle(delta_ms);
	}
	this->idleImpl(delta_ms);
}

void Angel::reshape(const geom::Area& area)
{
	this->renderArea_ = area;
	for(Handler<AngelTarget> const& t : this->targets_) {
		t->reshape(area);
	}
	this->reshapeImpl(area);
}

Handler< ::donut::Object> Angel::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(this->donutObject_.expired()){
		Handler< ::donut::Object> obj(createDonutObject(heap));
		this->donutObject_ = obj;
		return obj;
	}
	return this->donutObject_.lock();
}

Handler<AngelWidgetTarget> Angel::newWidgetTarget(std::string const& widgetId, std::string const& widgetGuide)
{
	return Handler<AngelWidgetTarget>(new AngelWidgetTarget(self(), widgetId, widgetGuide));
}
Handler<AngelElementTarget> Angel::newElementTarget(std::string const& elementId)
{
	return Handler<AngelElementTarget>(new AngelElementTarget(self(), elementId));
}

Handler<AngelWidgetTarget> Angel::findWidgetTarget(const std::string& widgetId, const std::string& widgetGuide )
{
	Handler<AngelWidgetTarget> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		if((res = target->matchToWidgetTarget(widgetId, widgetGuide))) {
			return res;
		}
	}
	return res;
}

Handler<AngelTarget> Angel::attatchTarget(const Handler<AngelTarget>& target)
{
	this->targets_.push_back(target);
	target->onAttatched();
	this->reshape(this->renderArea_);
	return target;
}

Handler<AngelTarget> Angel::detatchTarget(const Handler<AngelTarget>& target)
{
	auto it = std::find(std::begin(targets_), std::end(targets_), target);
	if(it != std::end(targets_)) {
		targets_.erase(it);
		target->onDetatched();
		return target;
	}
	CINAMO_EXCEPTION(Exception, "[BUG] Angel target not found.")
}

Handler<Element> Angel::findElementByPoint(const geom::Point& screenVector)
{
	Handler<Element> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		res = target->findElementByPoint(screenVector);
		if( res ) {
			break;
		}
	}
	return res;
}

Handler<Element> Angel::findElementById(const std::string& id)
{
	Handler<Element> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		res = target->findElementById(id);
		if( res ) {
			break;
		}
	}
	return res;
}

Handler<AngelElementTarget> Angel::findElementTarget(const std::string& elementId )
{
	Handler<AngelElementTarget> res;
	for(Handler<AngelTarget> const& target : this->targets_) {
		if((res = target->matchToElementTarget(elementId))) {
			return res;
		}
	}
	return res;
}
void Angel::onAttatched()
{
	this->onAttatchedImpl();
}
void Angel::onDetatched()
{
	this->onDetatchedImpl();
}

void Angel::onShown()
{
	this->onShownImpl();
	for(Handler<AngelTarget> const& target : this->targets_) {
		target->onShown();
	}
}
void Angel::onHidden()
{
	this->onHiddenImpl();
	for(Handler<AngelTarget> const& target : this->targets_) {
		target->onHidden();
	}
}
/**********************************************************************************************************************
 * Servants
 **********************************************************************************************************************/

AngelTarget::AngelTarget(const Handler<Angel>& angel)
:world_(angel->world())
,heaven_(angel->heaven())
,angel_(angel)
{
}

geom::Area AngelTarget::findScreenArea()
{
	return this->findScreenAreaImpl();
}

Handler<World> AngelTarget::world() const
{
	return this->world_.lock();
}
Handler<Heaven> AngelTarget::heaven() const
{
	return this->heaven_.lock();
}
Handler<Angel> AngelTarget::angel() const
{
	return this->angel_.lock();
}

void AngelTarget::attatchServant(const Handler<Servant>& servant)
{
	this->servants_.push_back(servant);
	servant->onAttatched();
	this->reshape( this->world()->area() );
}

bool AngelTarget::detatchServant(const Handler<Servant>& servant)
{
	auto it = std::find(this->servants_.begin(), this->servants_.end(), servant);
	if(it != this->servants_.end()) {
		this->servants_.erase(it);
		servant->onDetatched();
		return true;
	}else{
		return false;
	}
}
void AngelTarget::onAttatched()
{
	this->onAttatchedImpl();
}
void AngelTarget::onDetatched()
{
	this->onDetatchedImpl();
}
void AngelTarget::onShown()
{
	this->onShownImpl();
	for(Handler<Servant> const& serv : this->servants_) {
		serv->onShown();
	}
}
void AngelTarget::onHidden()
{
	this->onHiddenImpl();
	for(Handler<Servant> const& serv : this->servants_) {
		serv->onHidden();
	}
}

void AngelTarget::render(gl::Canvas& canvas)
{
	this->renderImpl(canvas);
	for(Handler<Servant> const& serv : this->servants_) {
		serv->render(canvas);
	}
}

void AngelTarget::idle(const float delta_ms)
{
	for(Handler<Servant> const& serv : this->servants_) {
		serv->idle(delta_ms);
	}
	this->idleImpl(delta_ms);
}

geom::Box AngelTarget::reshape(const geom::Area& area)
{
	this->renderArea_ = area;
	for(Handler<Servant> const& serv : this->servants_) {
		serv->reshape(area);
	}
	return this->reshapeImpl(area);
}

Handler<AngelWidgetTarget> AngelTarget::toWidgetTarget()
{
	return Handler<AngelWidgetTarget>();
}


Handler<AngelElementTarget> AngelTarget::toElementTarget()
{
	return Handler<AngelElementTarget>();
}

Handler<HaloServant> AngelTarget::newHaloServant( gl::Color const& color )
{
	return Handler<HaloServant>(new HaloServant(self(), color));
}
Handler<ElementServant> AngelTarget::newElementServant( Handler<Element> const& element )
{
	return Handler<ElementServant>(new ElementServant(self(), element));
}

Handler<ContentUnderlineServant> AngelTarget::newContentUnderlineServant(const gl::Color& color, std::string const& docId)
{
	return Handler<ContentUnderlineServant>(new ContentUnderlineServant(self(), color, docId));
}

Handler<ElementServant> AngelTarget::newElementServant( std::string const& elementId )
{
	Handler<World> world(this->world());
	if( unlikely(!world) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.")
	}
	Handler<Element> element = world->realizeElement(elementId);
	return Handler<ElementServant>(new ElementServant(self(), element));
}

Handler<AngelElementTarget> AngelTarget::matchToElementTarget(std::string const& elementId) noexcept
{
	return Handler<AngelElementTarget>();
}

Handler<AngelWidgetTarget> AngelTarget::matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept
{
	return Handler<AngelWidgetTarget>();
}

Handler< ::donut::Object> AngelTarget::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(this->donutObject_.expired()){
		Handler< ::donut::Object> obj(createDonutObject(heap));
		this->donutObject_ = obj;
		return obj;
	}
	return this->donutObject_.lock();
}


Handler<Element> AngelTarget::findElementByPoint(const geom::Point& screenVector)
{
	Handler<Element> elm;
	for( auto it = this->servants_.crbegin(); it != this->servants_.crend();++it ) {
		Handler<Servant> const& s = *it;
		elm = s->findElementByPoint(screenVector);
		if(elm) {
			break;
		}
	}
	return elm;
}

Handler<Element> AngelTarget::findElementById(const std::string& id)
{
	Handler<Element> elm;
	for(Handler<Servant> const& s : this->servants_){
		elm = s->findElementById(id);
		if(elm) {
			break;
		}
	}
	return elm;
}
/**********************************************************************************************************************
 * Servants
 **********************************************************************************************************************/

Servant::Servant(Handler<AngelTarget> const& angel_target)
:world_(angel_target->world())
,heaven_(angel_target->heaven())
,angel_(angel_target->angel())
,target_(angel_target)
{

}

Handler<World> Servant::world() const
{
	return this->world_.lock();
}
Handler<Heaven> Servant::heaven() const
{
	return this->heaven_.lock();
}

Handler<Angel> Servant::angel() const
{
	return this->angel_.lock();
}
Handler<AngelTarget> Servant::target() const
{
	return this->target_.lock();
}

Handler< ::donut::Object> Servant::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(this->donutObject_.expired()){
		Handler< ::donut::Object> obj(createDonutObject(heap));
		this->donutObject_ = obj;
		return obj;
	}
	return this->donutObject_.lock();
}



void Servant::render(gl::Canvas& canvas)
{
	this->renderImpl(canvas);
}

void Servant::idle(const float delta_ms)
{
	this->idleImpl(delta_ms);
}

geom::Box Servant::reshape(const geom::Area& area)
{
	this->renderArea_ = area;
	return this->reshapeImpl(area);
}

Handler<Element> Servant::findElementById(const std::string& id)
{
	return Handler<Element>();
}

Handler<Element> Servant::findElementByPoint(const geom::Point& screenVector)
{
	return Handler<Element>();
}

void Servant::onAttatched()
{
	this->onAttatchedImpl();
}
void Servant::onDetatched()
{
	this->onDetatchedImpl();
}
void Servant::onShown()
{
	this->onShownImpl();
}
void Servant::onHidden()
{
	this->onHiddenImpl();
}


}}

