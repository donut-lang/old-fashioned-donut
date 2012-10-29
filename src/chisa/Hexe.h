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
#include "logging/Logger.h"
#include "tk/layout/LayoutFactory.h"
#include "tk/widget/WidgetFactory.h"
#include "util/FileUtil.h"

namespace chisa {
class WorldGeist;

class Hexe {
	DISABLE_COPY_AND_ASSIGN(Hexe);
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, basepath);
public:
	Hexe(logging::Logger& log, const std::string& basepath);
	virtual ~Hexe();
	virtual std::string toString() const;
	template <typename... Args>
	constexpr std::string resolveFilepath(const Args&... path) const noexcept
	{
		return util::file::join(this->basepath(), path...);
	}

public:
	virtual void registerWidgets(tk::widget::WidgetFactory& factory){};
	virtual void registerLayouts(tk::layout::LayoutFactory& factory){};
public:
	virtual std::shared_ptr<WorldGeist> invokeWorldGeist(std::weak_ptr<chisa::tk::World> world, const std::string& nameOfGeist) = 0;
};

class WorldGeist {
	DISABLE_COPY_AND_ASSIGN(WorldGeist);
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	std::weak_ptr<chisa::tk::World> world_;
public:
	virtual std::string toString() const;
public:
	WorldGeist(logging::Logger& log, std::weak_ptr<chisa::tk::World> world);
	virtual ~WorldGeist();
public:
	inline std::shared_ptr<chisa::tk::World> world() { return world_.lock(); }
};

}
