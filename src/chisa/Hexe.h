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
#include <tarte/Logger.h>
#include <tarte/FileSystem.h>
#include <tarte/Handler.h>

#include "tk/element/ElementFactory.h"
#include "tk/widget/WidgetFactory.h"

namespace chisa {
using namespace tarte;
class WorldGeist;

class Hexe : public HandlerBody<Hexe> {
	DEFINE_MEMBER_REF(protected, Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, basepath);
public:
	Hexe(Logger& log, std::string const& basepath);
	virtual ~Hexe() noexcept;
	inline bool onFree() noexcept { return false; };
public:
	virtual std::string toString() const;
	template <typename... Args>
	constexpr std::string resolveFilepath(Args const&... path) const noexcept
	{
		return file::join(this->basepath(), path...);
	}

public:
	virtual void registerWidgets(tk::WidgetFactory& factory){};
	virtual void registerElements(tk::ElementFactory& factory){};
public:
	virtual Handler<WorldGeist> invokeWorldGeist(HandlerW<chisa::tk::World> world, std::string const& nameOfGeist) = 0;
};

class WorldGeist : public HandlerBody<WorldGeist> {
	DISABLE_COPY_AND_ASSIGN(WorldGeist);
	DEFINE_MEMBER_REF(protected, Logger, log);
	HandlerW<chisa::tk::World> world_;
public:
	virtual std::string toString() const;
public:
	WorldGeist(Logger& log, HandlerW<chisa::tk::World> world);
	virtual ~WorldGeist() noexcept;
	inline bool onFree() noexcept { return false; };
public:
	Handler<chisa::tk::World> world();
};

}
