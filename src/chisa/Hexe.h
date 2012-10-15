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

#ifndef _CXX_Chisa_APPLICATION_H_
#define _CXX_Chisa_APPLICATION_H_

#include <memory>
#include "logging/Logger.h"
#include "tk/layout/LayoutFactory.h"
#include "tk/widget/WidgetFactory.h"

namespace chisa {
class WorldGeist;

class Hexe {
	DISABLE_COPY_AND_ASSIGN(Hexe);
	DEFINE_MEMBER_REF(public, logging::Logger, log)
public:
	Hexe(logging::Logger& log);
	virtual ~Hexe();
	virtual std::string toString() const;
public:
	void registerWidgets(tk::widget::WidgetFactory& factory);
	void registerLayouts(tk::layout::LayoutFactory& factory);
public:
	WorldGeist* invokeWorldGeist(const std::string& nameOfGeist);
};

class WorldGeist {
	DISABLE_COPY_AND_ASSIGN(WorldGeist);
	DEFINE_MEMBER_REF(public, logging::Logger, log)
	virtual std::string toString() const;
public:
	WorldGeist(logging::Logger& log);
	virtual ~WorldGeist();
};

}
#endif /* INCLUDE_GUARD */
