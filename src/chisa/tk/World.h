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
#include "widget/WidgetFactory.h"
#include "layoyt/LayoutFactory.h"

namespace chisa {
namespace tk {
class Layout;
class Widget;
class WidgetHandler;

using std::shared_ptr;
using std::weak_ptr;
using std::string;

class World {
private:
	logging::Logger& log_;
	TaskHandler taskHandler_;
	Stack<shared_ptr<Layout> > layoutStack_;
	map<string, WidgetHandler*> widgetMap_;
private:
	layout::LayoutFactory* layoutFactory_;
	widget::WidgetFactory* widgetFactory_;
private:
	Box size_;
public:
	World(logging::Logger& log);
	virtual ~World();
	inline Box size() const{ return size_; };
public:
	void init(weak_ptr<World> _self, const string& worldname);
public:
	weak_ptr<Widget> getWidgetById(const string& id);
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const Box& area);
private:
	inline void size(const Box& newBox) { size_=newBox; };
	void popLayout();
	void pushLayout(const string& filename);
public:
	void replaceWidget(const string& widgetId, WidgetHandler* const newHandler);
	void deleteWidget(const string& widgetId, WidgetHandler* const handler);
};

}}
#endif /* INCLUDE_GUARD */
