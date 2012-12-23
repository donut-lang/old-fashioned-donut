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

#include "World.h"
#include "Element.h"
#include "element/ElementFactory.h"
#include "element/WidgetElement.h"
#include "widget/WidgetFactory.h"
#include "Universe.h"
#include "Gesture.h"
#include "../gl/DrawableManager.h"

namespace chisa {
namespace tk {

const static std::string& TAG("World");

World::World(Logger& log, HandlerW<Universe> _universe, std::string const& worldname)
:log_(log)
,universe_(_universe)
,name_(worldname)
,taskHandler_(log)
,doc_(nullptr)
,geist_(nullptr)
,elementFactory_(nullptr)
,widgetFactory_(nullptr)
,gestureMediator_(nullptr)
{

}

World::~World() noexcept
{
	if(this->doc_){
		delete this->doc_;
		this->doc_=nullptr;
	}
	if(this->elementFactory_){
		delete this->elementFactory_;
		this->elementFactory_ = nullptr;
	}
	if(this->widgetFactory_){
		delete this->widgetFactory_;
		this->widgetFactory_ = nullptr;
	}
	if(this->gestureMediator_){
		delete this->gestureMediator_;
		this->gestureMediator_ = nullptr;
	}
}


void World::init()
{
	if(Handler<Universe> universe = this->universe_.lock()){
		const std::string filename(universe->resolveWorldFilepath(this->name_, "layout.xml"));
		this->doc_ = new tinyxml2::XMLDocument();
		this->doc_->LoadFile(filename.c_str());
		this->elementFactory_ = new ElementFactory(this->log_, self(), filename, this->doc_, false);

		if( const char* geistName = this->doc_->RootElement()->Attribute("geist", nullptr)){
			universe->hexe()->registerElements(*this->elementFactory_);
			this->geist(universe->invokeWorldGeist(this->self(), geistName));
		}else{
			if(log().t()){
				log().t(TAG, "Geist not specified for: %s", this->name().c_str());
			}
		}
		this->widgetFactory_ = new WidgetFactory(this->log_, self());
		universe->hexe()->registerWidgets(*this->widgetFactory_);
	}
	this->gestureMediator_ = new GestureMediator(this->log_, self());
	this->pushElement("main");
}

void World::render(gl::Canvas& canvas)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->render(canvas, this->area(), geom::Area(0,0,this->area().width(), this->area().height()));
	}
}
void World::idle(const float delta_ms)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->idle(delta_ms);
	}
	this->taskHandler_.run(delta_ms);
}
void World::reshape(geom::Area const& area)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->measure(area.box());
		elm->layout(area.box());
	}
	this->area(area);
}

void World::pushElement(std::string const& filename)
{
	Handler<Element> l = this->elementFactory_->parseTree(filename);
	this->elementStack_.push(l);
}

void World::popElement()
{
	this->elementStack_.pop();
	if(Handler<Element> elm = this->elementStack_.top()){
		this->reshape(this->area());
	}
}

void World::registerTask(Task* task)
{
	this->taskHandler_.registerTask(task);
}

void World::unregisterTask(Task* task)
{
	this->taskHandler_.unregisterTask(task);
}

Handler<Element> World::getElementByPoint(geom::Point const& screenPoint)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		return elm->findElementByPoint(screenPoint);
	}
	return Handler<Element>();
}
Handler<Element> World::getElementById(std::string const& id)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		return elm->findElementById(id);
	}
	return Handler<Element>();
}

WidgetElement* World::getWidgetById(std::string const& name)
{
	auto it = this->widgetMap_.find(name);
	if(it == this->widgetMap_.end()){
		return nullptr;
	}
	return it->second;
}

bool World::replaceWidget(std::string const& widgetId, WidgetElement* const newHandler)
{
	std::unordered_map<std::string, WidgetElement*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		this->widgetMap_.erase(it);
	}
	this->widgetMap_.insert(std::pair<std::string, WidgetElement*>(widgetId, newHandler));
	return true;
}
bool World::deleteWidget(std::string const& widgetId, WidgetElement* const handler)
{
	std::unordered_map<std::string, WidgetElement*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		log_.w(TAG, "Oops. WidgetID: %s not found.", widgetId.c_str());
		return false;
	}
	if(it->second != handler) {
		log_.w(TAG, "Oops. WidgetID: %s query delete, but the handler is not correct.", widgetId.c_str());
		return false;
	}
	this->widgetMap_.erase(it);
	return true;
}

Widget* World::createWidget(std::string const& klass, tinyxml2::XMLElement* elem)
{
	return this->widgetFactory_->createWidget(klass, elem);
}

Handler<gl::DrawableManager> World::drawableManager() const
{
	if(Handler<Universe> uni = this->universe_.lock()){
		return uni->drawableManager();
	}else{
		return Handler<gl::DrawableManager>();
	}
}

void World::onTouchDown(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchDown(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchUp(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchUp(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchMove(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchMove(timeMs, pointerIndex, screenPoint);
	}
}

}}
