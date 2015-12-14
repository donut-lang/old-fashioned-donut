/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/Handler.h>
#include <cinamo/VectorMap.h>
#include <donut/Donut.hpp>
#include <string>
#include <cmath>
#include "../geom/Area.hpp"

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
using namespace cinamo;

namespace gl {
class Canvas;
}

namespace tk {
class ElementFactory;

class World;
class Angel;
class Element;

class Heaven final : public HandlerBody<Heaven> {
private:
	HandlerW<World> world_;
	std::vector<Handler<Angel> > angels_;
	HandlerW< ::donut::Object> donutObject_;
public:
	Heaven(Handler<World> const& world);
	virtual ~Heaven() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	Handler< ::donut::Object> donutObject(Handler< ::donut::Heap> const& heap);
public:
	inline HandlerW<World> const& world() const noexcept { return this->world_; };
public:
	void render(gl::Canvas& canvas);
	void idle(const float delta_ms);
	void reshape(geom::Area const& area);
public:
	Handler<Element> findElementByPoint(geom::Point const& screenVector);
	Handler<Element> findElementById(std::string const& id);
public:
	bool attatchAngel( Handler<Angel> const& angel );
	Handler<Angel> detatchAngel( Handler<Angel> const& angel );
public:
	Handler<Angel> newTwinAngel();
	Handler<Angel> newLoneAngel();
};


}}
