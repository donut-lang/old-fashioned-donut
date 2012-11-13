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
#include <unicode/unistr.h>
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"
#include "../geom/Decl.h"

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
namespace gl{
class Canvas;
}

namespace tk {
namespace element {
class WidgetWrapperLayout;
}

using std::shared_ptr;
using std::weak_ptr;

class Element;
class World;

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER(protected, private, weak_ptr<World>, world)
	DEFINE_MEMBER(private, private, weak_ptr<element::WidgetWrapperLayout>, wrapper);
public:
	Widget(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* element);
	virtual ~Widget();
public:
	void updateWrapper(weak_ptr<element::WidgetWrapperLayout> wrapper) { this->wrapper_ = wrapper; };
public:
	geom::Vector calcAbsolutePosition();
public:
	virtual void render(gl::Canvas& cv, const geom::Area& area) = 0;
	virtual void idle(const float delta_ms) = 0;
	virtual void reshape(const geom::Box& areaSize) = 0;
	virtual geom::Box measure(const geom::Box& constraintSize) = 0;
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInWidget){return false;};
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInWidget){return false;};
	virtual bool onMoveRaw(const float timeMs, const geom::Point& ptInWidget){return false;};
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInWidget){return false;};
	virtual bool onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity){return false;};
	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance){return false;};
	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio){return false;};

//	実装用
//public:
//	virtual void render(gl::Canvas& cv, const geom::Area& area) override;
//	virtual void idle(const float delta_ms) override;
//	virtual void reshape(const geom::Box& areaSize) override;
//	virtual geom::Box measure(const geom::Box& constraintSize) override;
//public:
//	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInWidget) override;
//	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInWidget) override;
//	virtual bool onMoveRaw(const float timeMs, const geom::Point& ptInWidget) override;
//	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInWidget) override;
//	virtual bool onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity) override;
//	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance) override;
//	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio) override;

};

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* element
#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_APPLY log, world, element

#define CHISA_WIDGET_SUBKLASS(Klass)\
public:\
Klass(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
virtual ~Klass();

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived) Derived(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_APPLY)

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Derived) \
Klass::Klass(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST)\
:CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(Klass) CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Widget)


}}
