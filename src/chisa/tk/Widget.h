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

namespace chisa {
namespace tk {
using std::shared_ptr;
using std::weak_ptr;

class Layout;

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
private:
	logging::Logger& log;
public:
	Widget(logging::Logger& log);
	virtual ~Widget();
public: /* レンダリング関連 */
	void invalidate();
public:
	Point calcAbsolutePosition();
	Point calcRelativePositionFromParent();
public:
	virtual void render(const Area& area) = 0;
	void idle(const float delta_ms);
	void reshape(const Box& area);
	virtual Box& measureSize(const Box& constraint) = 0;
};

}}
#endif /* INCLUDE_GUARD */
