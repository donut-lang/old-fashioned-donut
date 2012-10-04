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

World::World(logging::Logger& log, weak_ptr<World> self)
:log(log)
,self_(self)
,taskHandler(log)
{

}
World::~World()
{

}


void World::render()
{
	const Area area(0,0,this->size_.width(), this->size_.height());
	if(shared_ptr<Layout> layout = this->layoutStack.top()){
		layout->render(area);
	}
}
void World::idle(const float delta_ms)
{
	if(shared_ptr<Layout> layout = this->layoutStack.top()){
		layout->idle(delta_ms);
	}
}
void World::reshape(const Box& area)
{
	this->size(area);
	if(shared_ptr<Layout> layout = this->layoutStack.top()){
		layout->reshape(area);
	}
}

void World::pushLayout(const string& filename)
{
	layout::LayoutFactory factory(log, self_, filename);
	shared_ptr<Layout> l = factory.parseTree();
	this->layoutStack.push(l);
	this->reshape(this->size_);
}

void World::popLayout()
{
	this->layoutStack.pop();
	if(shared_ptr<Layout> layout = this->layoutStack.top()){
		if(!this->size().near(layout->size(), 1)){
			this->reshape(this->size());
		}
	}
}

void World::replaceWidget(const string& widgetId, WidgetHandler* const newHandler)
{
	map<string, WidgetHandler*>::iterator it = this->widgetMap.find(widgetId);
	if(it != widgetMap.end()) {
		this->widgetMap.erase(it);
	}
	this->widgetMap.insert(std::pair<string, WidgetHandler*>(widgetId, newHandler));
}
void World::deleteWidget(const string& widgetId, WidgetHandler* const handler)
{
	map<string, WidgetHandler*>::iterator it = this->widgetMap.find(widgetId);
	if(it != widgetMap.end()) {
		log.w(TAG, "Oops. WidgetID: %s not found.", widgetId.c_str());
		return;
	}
	if(it->second != handler) {
		log.w(TAG, "Oops. WidgetID: %s query delete, but the handler is not correct.", widgetId.c_str());
	}
	this->widgetMap.erase(it);
}

}}
