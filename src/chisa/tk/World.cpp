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
#include "layoyt/LayoutFactory.h"

namespace chisa {
namespace tk {

const static string& TAG("World");

World::World(logging::Logger& log)
:log_(log)
,taskHandler_(log)
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


void World::init(weak_ptr<World> _self, const string& worldname)
{
	this->layoutFactory_ = new layout::LayoutFactory(this->log_, _self, worldname);
}

void World::render()
{
	const Area area(0,0,this->size_.width(), this->size_.height());
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->render(area);
	}
}
void World::idle(const float delta_ms)
{
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->idle(delta_ms);
	}
}
void World::reshape(const Box& area)
{
	this->size(area);
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		layout->reshape(area);
	}
}

void World::pushLayout(const string& layoutname)
{
	shared_ptr<Layout> l = this->layoutFactory_->parseTree(layoutname);
	this->layoutStack_.push(l);
	this->reshape(this->size_);
}

void World::popLayout()
{
	this->layoutStack_.pop();
	if(shared_ptr<Layout> layout = this->layoutStack_.top()){
		if(!this->size().near(layout->size(), 1)){
			this->reshape(this->size());
		}
	}
}

void World::replaceWidget(const string& widgetId, WidgetHandler* const newHandler)
{
	map<string, WidgetHandler*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		this->widgetMap_.erase(it);
	}
	this->widgetMap_.insert(std::pair<string, WidgetHandler*>(widgetId, newHandler));
}
void World::deleteWidget(const string& widgetId, WidgetHandler* const handler)
{
	map<string, WidgetHandler*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		log_.w(TAG, "Oops. WidgetID: %s not found.", widgetId.c_str());
		return;
	}
	if(it->second != handler) {
		log_.w(TAG, "Oops. WidgetID: %s query delete, but the handler is not correct.", widgetId.c_str());
	}
	this->widgetMap_.erase(it);
}

}}
