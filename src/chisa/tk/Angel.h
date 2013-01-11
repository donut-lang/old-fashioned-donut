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
class Color;
class Canvas;
}

namespace tk {

class Heaven;
class World;
class Angel;
class Element;
class Widget;
class AngelTarget;

class HaloServant;
class ElementServant;

class Servant : public HandlerBody<Servant> {
private:
	HandlerW<World> world_;
	HandlerW<Heaven> heaven_;
	HandlerW<Angel> angel_;
	HandlerW<AngelTarget> target_;
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
	Handler<Angel> angel() const;
	Handler<AngelTarget> target() const;
protected:
	Servant(Handler<AngelTarget> const& angel_target);
public:
	virtual ~Servant() noexcept = default;
	inline bool onFree() const noexcept { return false; };
};


class AngelElementTarget;
class AngelWidgetTarget;

class AngelTarget : public HandlerBody<AngelTarget> {
protected:
	AngelTarget(Handler<Angel> const& angel);
public:
	virtual ~AngelTarget() noexcept = default;
	inline bool onFree() const noexcept { return false; };
private:
	HandlerW<World> const world_;
	HandlerW<Heaven> const heaven_;
	HandlerW<Angel> const angel_;
private:
	std::vector<Handler<Servant> > servants_;
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
	Handler<Angel> angel() const;
public:
	geom::Area findScreenArea();
	void attatchServant( Handler<Servant> const& servant );
public:
	Handler<HaloServant> newHaloServant( gl::Color const& color );
	Handler<ElementServant> newElementServant( Handler<Element> const& element );
public:
	virtual Handler<AngelElementTarget> matchToElementTarget(std::string const& elementId) noexcept;
	virtual Handler<AngelWidgetTarget> matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept;
protected:
	virtual geom::Area findScreenAreaImpl() = 0;
};

class Angel : public HandlerBody<Angel> {
private:
	HandlerW<World> world_;
	HandlerW<Heaven> heaven_;
private:
	std::vector<Handler<AngelTarget> > targets_;
public:
	Angel(Handler<Heaven> heaven);
	virtual ~Angel() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
public:
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	void reshape(geom::Area const& area);
protected:
	virtual void renderImpl(gl::Canvas& canvas) = 0;
	virtual void idleImpl(const float delta_ms) = 0;
	virtual void reshapeImpl(geom::Area const& area) = 0;
public:
	Handler<AngelWidgetTarget> newWidgetTarget(std::string const& widgetId, std::string const& widgetGuide);
	Handler<AngelElementTarget> newElementTarget(std::string const& elementId);
	Handler<AngelWidgetTarget> findWidgetTarget(const std::string& widgetId, const std::string& widgetGuide);
	Handler<AngelElementTarget> findElementTarget(const std::string& elementId);
};

}}
