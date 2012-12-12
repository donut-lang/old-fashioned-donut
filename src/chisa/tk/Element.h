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
#include "Widget.h"
#include "Gesture.h"
#include "../util/ClassUtil.h"
#include "../geom/Vector.h"
#include "../geom/Area.h"
#include <string>
#include <cmath>

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
namespace tk {
namespace element {
class ElementFactory;
}

class World;

class Element : public GestureListener {
	DISABLE_COPY_AND_ASSIGN(Element);
private:
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER(protected, private, std::weak_ptr<World>, world);
	DEFINE_MEMBER(public, private, std::weak_ptr<Element>, root);
	DEFINE_MEMBER(public, private, std::weak_ptr<Element>, parent);
	DEFINE_MEMBER(protected, private, std::weak_ptr<Element>, self);
	DEFINE_MEMBER(public, private, geom::Box, size);
	DEFINE_MEMBER(public, private, geom::Area, screenArea);
	DEFINE_MEMBER(protected, private, geom::Area, drawnArea);
	DEFINE_MEMBER(public, private, std::string, id);
public:
	virtual std::weak_ptr<Element> getChildAt(const size_t index) const = 0;
	virtual size_t getChildCount() const = 0;
public:
	void render(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area);
	geom::Box measure(geom::Box const& constraint);
	void layout(geom::Box const& size);
	virtual std::string toString() const = 0;
	void loadXML(element::ElementFactory* const factory, tinyxml2::XMLElement* const element);
	std::weak_ptr<Element> getElementById(std::string const& id);
	std::weak_ptr<Element> getElementByPoint(geom::Vector const& screenPoint);
	virtual void idle(const float delta_ms);
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) = 0;
	virtual geom::Box onMeasure(geom::Box const& constraint) = 0;
	virtual void onLayout(geom::Box const& size) = 0;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual std::weak_ptr<Element> getElementByIdImpl(std::string const& id) = 0;
protected:
	Element(logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent);
private:
	void init(std::weak_ptr<Element> _self);
public:
	template <typename SubKlass>
	static std::shared_ptr<SubKlass> create(logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent)
	{
		std::shared_ptr<SubKlass> ptr(new SubKlass(log, world, root, parent));
		ptr->init(ptr);
		return ptr;
	}
	virtual ~Element() noexcept = default;
public:
	inline static float updateMax(const float a, const float b)
	{
		return std::isnan(b) ? a : (a > b ? a : b);
	}
	inline static float updateMin(const float a, const float b)
	{
		return std::isnan(a) ? b : (a > b ? b : a);
	}
};


#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent
#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY log, world, root, parent

#define CHISA_ELEMENT_SUBKLASS_FINAL(Klass) \
friend std::shared_ptr<Klass> Element::create<Klass>(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
private:\
	Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
public:\
	virtual ~Klass() noexcept;

#define CHISA_ELEMENT_SUBKLASS(Klass) \
protected:\
	Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
public:\
	virtual ~Klass() noexcept;

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived) Derived(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY)

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Derived) \
Klass::Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)\
:CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(Klass) CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Element)

}}
