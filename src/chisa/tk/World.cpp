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
#include "Layout.h"
#include "layout/LayoutFactory.h"
#include "layout/WidgetWrapperLayout.h"
#include "widget/WidgetFactory.h"
#include "Universe.h"
#include "Gesture.h"

namespace chisa {
namespace tk {

const static string& TAG("World");

World::World(logging::Logger& log, weak_ptr<Universe> _universe, const string& worldname)
:log_(log)
,universe_(_universe)
,name_(worldname)
,taskHandler_(log)
,doc_(nullptr)
,geist_(nullptr)
,layoutFactory_(nullptr)
,widgetFactory_(nullptr)
,gestureMediator_(nullptr)
{

}

World::~World()
{
	if(this->doc_){
		delete this->doc_;
		this->doc_=nullptr;
	}
	if(this->layoutFactory_){
		delete this->layoutFactory_;
		this->layoutFactory_ = nullptr;
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


void World::init(weak_ptr<World> _self)
{
	if(shared_ptr<Universe> universe = this->universe_.lock()){
		const std::string filename(universe->resolveWorldFilepath(this->name_, "layout.xml"));
		this->doc_ = new tinyxml2::XMLDocument();
		this->doc_->LoadFile(filename.c_str());
		this->layoutFactory_ = new layout::LayoutFactory(this->log_, _self, filename, this->doc_, false);

		if( const char* geistName = this->doc_->RootElement()->Attribute("geist", nullptr)){
			universe->hexe()->registerLayouts(*this->layoutFactory_);
			this->geist(universe->invokeWorldGeist(_self, geistName));
		}else{
			if(log().t()){
				log().t(TAG, "Geist not specified for: %s", this->name().c_str());
			}
		}
		this->widgetFactory_ = new widget::WidgetFactory(this->log_, _self);
		universe->hexe()->registerWidgets(*this->widgetFactory_);
	}
	this->gestureMediator_ = new GestureMediator(this->log_, _self);
	this->pushLayout("main");
}

void World::render(gl::Canvas& canvas)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->render(canvas, this->area(), geom::Area(0,0,this->area().width(), this->area().height()));
	}
}
void World::idle(const float delta_ms)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->idle(delta_ms);
	}
	this->taskHandler_.run(delta_ms);
}
void World::reshape(const geom::Area& area)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->measure(area.box());
		layout->layout(area.box());
	}
	this->area(area);
}

void World::pushLayout(const string& layoutname)
{
	shared_ptr<Layout> l = this->layoutFactory_->parseTree(layoutname);
	this->layoutStack_.push(l);
}

void World::popLayout()
{
	this->layoutStack_.pop();
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
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

weak_ptr<Layout> World::getLayoutByPoint(const geom::Point& screenPoint)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		return layout->getLayoutByPoint(screenPoint);
	}
	return weak_ptr<Layout>();
}

layout::WidgetWrapperLayout* World::getWidgetById(const std::string& name)
{
	auto it = this->widgetMap_.find(name);
	if(it == this->widgetMap_.end()){
		return nullptr;
	}
	return it->second;
}

bool World::replaceWidget(const string& widgetId, layout::WidgetWrapperLayout* const newHandler)
{
	std::map<std::string, layout::WidgetWrapperLayout*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		this->widgetMap_.erase(it);
	}
	this->widgetMap_.insert(std::pair<string, layout::WidgetWrapperLayout*>(widgetId, newHandler));
	return true;
}
bool World::deleteWidget(const string& widgetId, layout::WidgetWrapperLayout* const handler)
{
	std::map<std::string, layout::WidgetWrapperLayout*>::iterator it = this->widgetMap_.find(widgetId);
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

Widget* World::createWidget(const string& klass, tinyxml2::XMLElement* elem)
{
	return this->widgetFactory_->createWidget(klass, elem);
}

Handler<gl::RawSprite> World::queryImage(const string& abs_filename)
{
	if(std::shared_ptr<Universe> uni = this->universe_.lock()){
		return uni->queryImage(abs_filename);
	}else{
		return Handler<gl::RawSprite>();
	}
}

Handler<gl::RawSprite> World::queryRawSprite(const int width, const int height)
{
	if(std::shared_ptr<Universe> uni = this->universe_.lock()){
		return uni->queryRawSprite(width, height);
	}else{
		return Handler<gl::RawSprite>();
	}
}

Handler<gl::FontManager> World::fontManager() const
{
	if(std::shared_ptr<Universe> uni = this->universe_.lock()){
		return uni->fontManager();
	}else{
		return Handler<gl::FontManager>();
	}
}

void World::onTouchDown(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchDown(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchUp(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchUp(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchMove(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchMove(timeMs, pointerIndex, screenPoint);
	}
}

}}
