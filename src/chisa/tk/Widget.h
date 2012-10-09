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

#ifndef Chisa_TK_WIDGET_H__CPP_
#define Chisa_TK_WIDGET_H__CPP_

#include <tr1/memory>
#include <unicode/unistr.h>
#include "../util/class_utils.h"
#include "../logging/Logger.h"
#include "./Geom.h"

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
namespace gl{
class Canvas;
}

namespace tk {
namespace layout {
class WidgetWrapperLayout;
}

using std::shared_ptr;
using std::weak_ptr;

class Layout;
class World;

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER(protected, private, weak_ptr<World>, world)
	DEFINE_MEMBER(private, private, weak_ptr<layout::WidgetWrapperLayout>, wrapper);
public:
	Widget(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* element);
	virtual ~Widget();
public:
	void updateWrapper(weak_ptr<layout::WidgetWrapperLayout> wrapper) { this->wrapper_ = wrapper; };
public:
	Point calcAbsolutePosition();
public:
	virtual void render(gl::Canvas& cv, const Area& area) = 0;
	virtual void idle(const float delta_ms) = 0;
	virtual void reshape(const Box& area) = 0;
	virtual Box measure(const Box& constraint) = 0;
};

}}
#endif /* INCLUDE_GUARD */
