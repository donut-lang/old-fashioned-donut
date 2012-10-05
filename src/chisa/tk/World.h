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

#ifndef Chisa_TK_WORLD_H__CPP_
#define Chisa_TK_WORLD_H__CPP_
#include <memory>
#include <string>
#include <map>

#include "Task.h"
#include "Geom.h"
#include "Stack.h"

namespace chisa {
namespace tk {
class Layout;
class Widget;
class Universe;

namespace layout {
class WidgetWrapperLayout;
class LayoutFactory;
}
namespace widget{
class WidgetFactory;
}

using std::shared_ptr;
using std::weak_ptr;
using std::string;

class World {
private:
	logging::Logger& log_;
	const weak_ptr<Universe> universe_;
	const std::string name_;
	TaskHandler taskHandler_;
	Stack<shared_ptr<Layout> > layoutStack_;
	map<string, layout::WidgetWrapperLayout*> widgetMap_;
private:
	layout::LayoutFactory* layoutFactory_;
	widget::WidgetFactory* widgetFactory_;
private:
	Area area_;
public:
	World(logging::Logger& log, weak_ptr<Universe> _universe, const string& worldname);
	virtual ~World();
	inline Area area() const{ return area_; };
public:
	void init(weak_ptr<World> _self);
public:
	weak_ptr<Widget> getWidgetById(const string& id);
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const Area& area);
private:
	inline void area(const Area& newArea) { area_=newArea; };
	void popLayout();
	void pushLayout(const string& filename);
public:
	bool replaceWidget(const string& widgetId, layout::WidgetWrapperLayout* const newHandler);
	bool deleteWidget(const string& widgetId, layout::WidgetWrapperLayout* const handler);
};

}}
#endif /* INCLUDE_GUARD */
