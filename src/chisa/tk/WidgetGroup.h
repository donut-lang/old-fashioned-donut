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

#ifndef Chisa_TK_WIDGETGROUP_H__CPP_
#define Chisa_TK_WIDGETGROUP_H__CPP_

#include "Widget.h"
#include "../util/class_utils.h"

namespace chisa {
namespace tk {

class WidgetGroup: public chisa::tk::Widget {
	DISABLE_COPY_AND_ASSIGN(WidgetGroup);
public:
	std::weak_ptr<Widget> getChildAt(const size_t index) const;
	std::size_t getChildCount() const;
	std::size_t getChildIndexOf(std::weak_ptr<Widget> dancer);
	weak_ptr<Widget> getWidgetById(const UnicodeString& id);
public:
	virtual ~WidgetGroup();
};

}}
#endif /* INCLUDE_GUARD */
