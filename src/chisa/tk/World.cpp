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

namespace chisa {
namespace tk {

const static string& TAG("World");

World::World(logging::Logger& log, weak_ptr<Universe> _universe, const string& worldname)
:log_(log)
,universe_(_universe)
,name_(worldname)
,taskHandler_(log)
,layoutFactory_(nullptr)
,widgetFactory_(nullptr)
{

}
World::~World()
{
	if(this->layoutFactory_){
		delete this->layoutFactory_;
		this->layoutFactory_ = nullptr;
	}
	if(this->widgetFactory_){
		delete this->widgetFactory_;
		this->widgetFactory_ = nullptr;
	}
}


void World::init(weak_ptr<World> _self)
{
	if(shared_ptr<Universe> universe = this->universe_.lock()){
		this->layoutFactory_ = new layout::LayoutFactory(this->log_, _self, universe->resolveWorldFilepath(this->name_, "layout.xml"));
	}
	this->pushLayout("main");
}

void World::render()
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->render(this->area());
	}
}
void World::idle(const float delta_ms)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->idle(delta_ms);
	}
}
void World::reshape(const Area& area)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->reshape(area);
	}
	this->area(area);
}

void World::pushLayout(const string& layoutname)
{
	shared_ptr<Layout> l = this->layoutFactory_->parseTree(layoutname);
	this->layoutStack_.push(l);
	this->reshape(this->area());
}

void World::popLayout()
{
	this->layoutStack_.pop();
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		if(!this->area().near(layout->area(), 1)){
			this->reshape(this->area());
		}
	}
}

Widget* World::getWidgetById(const std::string& name)
{
	auto it = this->widgetMap_.find(name);
	if(it == this->widgetMap_.end()){
		return nullptr;
	}
	return it->second->widget();
}

bool World::replaceWidget(const string& widgetId, layout::WidgetWrapperLayout* const newHandler)
{
	map<string, layout::WidgetWrapperLayout*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		this->widgetMap_.erase(it);
	}
	this->widgetMap_.insert(std::pair<string, layout::WidgetWrapperLayout*>(widgetId, newHandler));
	return true;
}
bool World::deleteWidget(const string& widgetId, layout::WidgetWrapperLayout* const handler)
{
	map<string, layout::WidgetWrapperLayout*>::iterator it = this->widgetMap_.find(widgetId);
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

}}
