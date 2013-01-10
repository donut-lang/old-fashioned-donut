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

#include <string>
#include <cmath>
#include <tarte/ClassUtil.h>
#include <tarte/Handler.h>
#include <tarte/VectorMap.h>
#include "../geom/Area.h"

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
using namespace tarte;

namespace gl {
class Canvas;
}

namespace tk {

class Heaven;
class World;
class Angel;
class Element;
class Widget;

class AngelTarget : public HandlerBody<AngelTarget> {
protected:
	AngelTarget(Handler<Angel> const& angel);
	virtual ~AngelTarget() noexcept = default;
private:
	HandlerW<World> const world_;
	HandlerW<Heaven> const heaven_;
	HandlerW<Angel> const angel_;
public:
	inline HandlerW<World> const& world() const noexcept { return this->world_; };
	inline HandlerW<Heaven> const& heaven() const noexcept { return this->heaven_; };
	inline HandlerW<Angel> const& angel() const noexcept { return this->angel_; };
public:
	geom::Area findScreenArea();
protected:
	virtual geom::Area findScreenAreaImpl() = 0;
};

class Angel : public HandlerBody<Angel> {
private:
	HandlerW<World> world_;
	HandlerW<Heaven> heaven_;
public:
	Angel(Handler<Heaven> heaven);
	virtual ~Angel() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	inline HandlerW<World> const& world() const noexcept { return this->world_; };
	inline HandlerW<Heaven> const& heaven() const noexcept { return this->heaven_; };
public:
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	void reshape(geom::Area const& area);
};


}}
