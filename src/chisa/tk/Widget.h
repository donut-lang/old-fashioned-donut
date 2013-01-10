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
#include <tarte/Handler.h>
#include <tarte/ClassUtil.h>
#include <tarte/Logger.h>
//#include "donut/WidgetObject.h"
#include "../geom/Area.h"

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
using namespace tarte;

namespace gl{
class Canvas;
}

namespace tk {

class WidgetElement;
class Element;
class World;

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
	DEFINE_MEMBER_REF(protected, Logger, log);
	DEFINE_MEMBER(protected, private, HandlerW<World>, world)
	DEFINE_MEMBER(protected, private, HandlerW<WidgetElement>, wrapper);
private: /* どーなつとの接続 */
//	HandlerW<WidgetObject> donutObject_;
private:
	geom::Box size_;
	geom::Area lastDrawnArea_;
public:
	Widget(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element);
	virtual ~Widget() noexcept = default;
public:
	void updateWrapper(HandlerW<WidgetElement> wrapper) { this->wrapper_ = wrapper; };
public:
	void render(gl::Canvas& cv, geom::Area const& area);
	void idle(const float delta_ms);
	void reshape(geom::Box const& areaSize);
	geom::Box measure(geom::Box const& constraintSize);
public:
	geom::Area findTarget(std::string const& target);
	geom::Area findTargetInRoot(std::string const& target);
	geom::Area findTargetInElement(std::string const& target);
protected:
	virtual void renderImpl(gl::Canvas& cv, geom::Area const& area) = 0;
	virtual void idleImpl(const float delta_ms) = 0;
	virtual void reshapeImpl(geom::Box const& areaSize) = 0;
	virtual geom::Box measureImpl(geom::Box const& constraintSize) = 0;
protected:
	virtual geom::Area findTargetImpl(std::string const& target) { return lastDrawnArea_; };
public:
	void notifyRelayoutFinished();
protected:
	virtual void notifyRelayoutFinishedImpl() {};
	void requestRelayout();
	void notifyViewRefreshed();
public:
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInWidget){return false;};
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInWidget){return false;};
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInWidget){return false;};
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInWidget){return false;};
	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity){return false;};
	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance){return false;};
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio){return false;};

//	実装用
//public:
//	virtual void render(gl::Canvas& cv, geom::Area const& area) override;
//	virtual void idle(const float delta_ms) override;
//	virtual void reshape(geom::Box const& areaSize) override;
//	virtual geom::Box measure(geom::Box const& constraintSize) override;
//public:
//	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInWidget) override;
//	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInWidget) override;
//	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInWidget) override;
//	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInWidget) override;
//	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity) override;
//	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override;
//	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override;

};

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element
#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_APPLY log, world, element

#define CHISA_WIDGET_SUBKLASS(Klass)\
public:\
Klass(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
virtual ~Klass() noexcept = default;

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived) Derived(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_APPLY)

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Derived) \
Klass::Klass(CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_PARAM_LIST)\
:CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)

#define CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(Klass) CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Widget)


}}
