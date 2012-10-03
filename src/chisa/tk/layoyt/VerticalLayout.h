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

#ifndef Chisa_TK_LAYOUT_VERTICALLAYOUT_H__CPP_
#define Chisa_TK_LAYOUT_VERTICALLAYOUT_H__CPP_

#include "SplitLayout.h"

namespace chisa {
namespace tk {
namespace layout {

class VerticalLayout: public chisa::tk::layout::SplitLayout {
public:
	VerticalLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
	virtual ~VerticalLayout();
	virtual string toString();
public:
	virtual void render(const Area& area);
	virtual void reshape(const Box& area);
};

}}}
#endif /* INCLUDE_GUARD */
