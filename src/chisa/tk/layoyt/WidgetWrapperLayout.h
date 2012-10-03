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

#ifndef Chisa_WIDGETWRAPPERLAYOUT_H__CPP_
#define Chisa_WIDGETWRAPPERLAYOUT_H__CPP_

#include "../Layout.h"
#include <memory>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace layout {
class LayoutFactory;
using namespace std;
using namespace tinyxml2;

class WidgetWrapperLayout: public chisa::tk::Layout {
public:
	WidgetWrapperLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
	virtual ~WidgetWrapperLayout();
public:
	static WidgetWrapperLayout* parseTree(LayoutFactory& factory, weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top);
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const;
	virtual size_t getChildCount() const;
	virtual weak_ptr<Widget> getWidgetById(const string& id);
public:
	virtual void render(const Area& area);
	virtual void idle(const float delta_ms);
	virtual Box measure(const Box& constraint);
	virtual void reshape(const Box& area);
};

}}}
#endif /* INCLUDE_GUARD */
