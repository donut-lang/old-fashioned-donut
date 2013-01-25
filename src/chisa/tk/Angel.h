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
#include <donut/Donut.h>
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
class ContentUnderlineServant;

class Servant : public HandlerBody<Servant> {
private:
	geom::Area renderArea_;
private:
	HandlerW<World> world_;
	HandlerW<Heaven> heaven_;
	HandlerW<Angel> angel_;
	HandlerW<AngelTarget> target_;
private:
	HandlerW< ::donut::Object> donutObject_;
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
	Handler<Angel> angel() const;
	Handler<AngelTarget> target() const;
	Handler< ::donut::Object> donutObject(Handler< ::donut::Heap> const& heap);
protected:
	Servant(Handler<AngelTarget> const& angel_target);
	inline geom::Area const& renderArea() const noexcept { return this->renderArea_; };
public:
	virtual ~Servant() noexcept = default;
	inline bool onFree() const noexcept { return false; };
protected:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) = 0;
public:
	void onAttatched();
	void onDetatched();
	void onShown();
	void onHidden();
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	geom::Box reshape(geom::Area const& area);
public:
	virtual Handler<Element> findElementById(std::string const& id);
	virtual Handler<Element> findElementByPoint(geom::Point const& screenVector);
protected:
	virtual void onAttatchedImpl() {};
	virtual void onDetatchedImpl() {};
	virtual void onShownImpl() {};
	virtual void onHiddenImpl() {};
	virtual void renderImpl(gl::Canvas& canvas) = 0;
	virtual void idleImpl(const float delta_ms) = 0;
	virtual geom::Box reshapeImpl(geom::Area const& area) = 0;
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
	HandlerW< ::donut::Object> donutObject_;
private:
	geom::Area renderArea_;
	std::vector<Handler<Servant> > servants_;
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
	Handler<Angel> angel() const;
	Handler< ::donut::Object> donutObject(Handler< ::donut::Heap> const& heap);
	std::size_t inline numServants() const noexcept { return this->servants_.size(); };
protected:
	inline std::vector<Handler<Servant> >& servants() noexcept { return this->servants_; };
	inline geom::Area const& renderArea() const noexcept { return this->renderArea_; };
public:
	geom::Area findScreenArea();
	void attatchServant( Handler<Servant> const& servant );
	bool detatchServant( Handler<Servant> const& servant );
public:
	Handler<HaloServant> newHaloServant( gl::Color const& color );
	Handler<ContentUnderlineServant> newContentUnderlineServant( gl::Color const& color, std::string const& docId );
	Handler<ElementServant> newElementServant( Handler<Element> const& element );
	Handler<ElementServant> newElementServant( std::string const& elemId );
public:
	virtual Handler<AngelElementTarget> matchToElementTarget(std::string const& elementId) noexcept;
	virtual Handler<AngelWidgetTarget> matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept;
public:
	void onAttatched();
	void onDetatched();
	void onShown();
	void onHidden();
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	geom::Box reshape(geom::Area const& area);
public:
	Handler<Element> findElementByPoint(geom::Point const& screenVector);
	Handler<Element> findElementById(std::string const& id);
protected:
	virtual void onAttatchedImpl() {};
	virtual void onDetatchedImpl() {};
	virtual void onShownImpl() {};
	virtual void onHiddenImpl() {};
	virtual void renderImpl(gl::Canvas& canvas) = 0;
	virtual void idleImpl(const float delta_ms) = 0;
	virtual geom::Box reshapeImpl(geom::Area const& area) = 0;
protected:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) = 0;
	virtual geom::Area findScreenAreaImpl() = 0;
public:
	virtual Handler<AngelWidgetTarget> toWidgetTarget();
	virtual Handler<AngelElementTarget> toElementTarget();
};

class Angel : public HandlerBody<Angel> {
private:
	HandlerW<World> world_;
	HandlerW<Heaven> heaven_;
private:
	HandlerW< ::donut::Object> donutObject_;
private:
	geom::Area renderArea_;
	std::vector<Handler<AngelTarget> > targets_;
protected:
	Angel(Handler<Heaven> heaven);
public:
	virtual ~Angel() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	Handler<World> world() const;
	Handler<Heaven> heaven() const;
	Handler< ::donut::Object> donutObject(Handler< ::donut::Heap> const& heap);
public:
	void onAttatched();
	void onDetatched();
	void onShown();
	void onHidden();
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	void reshape(geom::Area const& area);
public:
	Handler<Element> findElementByPoint(geom::Point const& screenVector);
	Handler<Element> findElementById(std::string const& id);
protected:
	virtual void onAttatchedImpl() {};
	virtual void onDetatchedImpl() {};
	virtual void onShownImpl() {};
	virtual void onHiddenImpl() {};
	virtual void renderImpl(gl::Canvas& canvas) = 0;
	virtual void idleImpl(const float delta_ms) = 0;
	virtual void reshapeImpl(geom::Area const& area) = 0;
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) = 0;
public:
	inline std::size_t numTargets() const noexcept { return this->targets_.size(); };
protected:
	inline std::vector<Handler<AngelTarget> >& targets() noexcept { return this->targets_; };
	inline geom::Area const& renderArea() const noexcept { return this->renderArea_; };
public:
	virtual Handler<AngelTarget> attatchTarget(Handler<AngelTarget> const& target);
	virtual Handler<AngelTarget> detatchTarget(Handler<AngelTarget> const& target);
	Handler<AngelWidgetTarget> newWidgetTarget(std::string const& widgetId, std::string const& widgetGuide);
	Handler<AngelElementTarget> newElementTarget(std::string const& elementId);
	Handler<AngelWidgetTarget> findWidgetTarget(const std::string& widgetId, const std::string& widgetGuide);
	Handler<AngelElementTarget> findElementTarget(const std::string& elementId);
};

}}
