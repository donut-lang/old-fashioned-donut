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

#pragma once
#include <memory>
#include <string>
#include <map>
#include "../util/ClassUtil.h"
#include "../gl/Canvas.h"
#include "../geom/Area.h"

#include "Task.h"
#include "Stack.h"
#include "Universe.h"

namespace tinyxml2{
class XMLElement;
class XMLDocument;
}

namespace chisa {

namespace tk {
class Element;
class Widget;
class GestureMediator;

namespace element {
class WidgetElement;
class ElementFactory;
}
namespace widget{
class WidgetFactory;
}

using std::shared_ptr;
using std::weak_ptr;
using std::string;

class World {
private:
	DEFINE_MEMBER_REF(public, logging::Logger, log);
	const weak_ptr<Universe> universe_;
	DEFINE_MEMBER_CONST(public, std::string, name);
	TaskHandler taskHandler_;
	Stack<shared_ptr<Element> > layoutStack_;
	std::map<std::string, element::WidgetElement*> widgetMap_;
	DEFINE_MEMBER(private, private, geom::Area, area);
private:
	tinyxml2::XMLDocument* doc_;
	DEFINE_MEMBER(public, private, std::shared_ptr<WorldGeist>, geist);
private:
	element::ElementFactory* layoutFactory_;
	widget::WidgetFactory* widgetFactory_;
	GestureMediator* gestureMediator_;
public:
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	void reshape(const geom::Area& area);
private:
	void popLayout();
	void pushLayout(const string& filename);
public:
	element::WidgetElement* getWidgetById(const std::string& name);
	bool replaceWidget(const string& widgetId, element::WidgetElement* const newHandler);
	bool deleteWidget(const string& widgetId, element::WidgetElement* const handler);
	Widget* createWidget(const string& klass, tinyxml2::XMLElement* elem);
public:
	weak_ptr<Element> getLayoutByPoint(const geom::Point& screenVector);
	/******************************************************************************
	 * タスク管理
	 ******************************************************************************/
public:
	void registerTask(Task* task);
	void unregisterTask(Task* task);
	/******************************************************************************
	 * Universeへの移譲
	 ******************************************************************************/
public:
	Handler<gl::DrawableManager> drawableManager() const;
	template <typename... Args>
	std::string resolveWorldFilepath(const Args&... related_filename) const
	{
		if(shared_ptr<Universe> universe = this->universe_.lock()){
			return universe->resolveWorldFilepath(this->name(), related_filename...);
		}else{
			throw logging::Exception(__FILE__, __LINE__, "Oops. Universe already removed.");
		}
	}
	template <typename... Args>
	std::string resolveUniverseFilepath(const Args&... related_filename) const
	{
		if(shared_ptr<Universe> universe = this->universe_.lock()){
			return universe->resolveUniverseFilepath(related_filename...);
		}else{
			throw logging::Exception(__FILE__, __LINE__, "Oops. Universe already removed.");
		}
	}
	/******************************************************************************
	 * タッチイベント。Universeから移譲されてくる
	 ******************************************************************************/
public:
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenVector);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenVector);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenVector);

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
