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
#include "./Geom.h"

namespace chisa {
namespace tk {
using std::shared_ptr;
using std::weak_ptr;

class World;
class WidgetGroup;

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
private:
	weak_ptr<World> world_;
	weak_ptr<WidgetGroup> root_;
	weak_ptr<WidgetGroup> parent_;
protected:
	weak_ptr<World> world() { return world_; };
	weak_ptr<WidgetGroup> root() { return root_; };
	weak_ptr<WidgetGroup> parent() { return parent_; };
public:
	Widget(weak_ptr<World> world, weak_ptr<WidgetGroup> root, weak_ptr<WidgetGroup> parent);
	virtual ~Widget();
public: /* レンダリング関連 */
	void invalidate();
public:
	Point calcAbsolutePosition();
	Point calcRelativePositionFromParent();
public:
	void render(const Area& area); /* OpenGLの座標の設定などを行なってしまう */
	virtual void renderImpl() = 0;
	void idle(const float delta_ms);
	void reshape(const Box& area);
};

}}
#endif /* INCLUDE_GUARD */
