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

namespace tinyxml2{
class XMLElement;
}

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
	inline Area area() const{ return area_; };
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const Area& area);
private:
	inline void area(const Area& newArea) { area_=newArea; };
	void popLayout();
	void pushLayout(const string& filename);
public:
	layout::WidgetWrapperLayout* getWidgetById(const std::string& name);
	bool replaceWidget(const string& widgetId, layout::WidgetWrapperLayout* const newHandler);
	bool deleteWidget(const string& widgetId, layout::WidgetWrapperLayout* const handler);
	Widget* createWidget(const string& klass, tinyxml2::XMLElement* elem);

	/******************************************************************************
	 * 生成
	 ******************************************************************************/
private:
	World(logging::Logger& log, weak_ptr<Universe> _universe, const string& worldname);
	void init(weak_ptr<World> _self);
public:
	static shared_ptr<World> create(logging::Logger& log, weak_ptr<Universe> _universe, const string& worldname)
	{
		shared_ptr<World> ptr(new World(log, _universe, worldname));
		ptr->init(ptr);
		return ptr;
	}
	virtual ~World();
};

}}
#endif /* INCLUDE_GUARD */
