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
#include <memory>
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
using std::string;
using std::weak_ptr;
using std::size_t;

class Element : public GestureListener {
	DISABLE_COPY_AND_ASSIGN(Element);
private:
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER(protected, private, weak_ptr<World>, world);
	DEFINE_MEMBER(public, private, weak_ptr<Element>, root);
	DEFINE_MEMBER(public, private, weak_ptr<Element>, parent);
	DEFINE_MEMBER(protected, private, weak_ptr<Element>, self);
	DEFINE_MEMBER(public, private, geom::Box, size);
	DEFINE_MEMBER(public, private, geom::Area, screenArea);
	DEFINE_MEMBER(protected, private, geom::Area, drawnArea);
	DEFINE_MEMBER(public, private, std::string, id);
public:
	virtual weak_ptr<Element> getChildAt(const size_t index) const = 0;
	virtual size_t getChildCount() const = 0;
public:
	void render(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area);
	geom::Box measure(const geom::Box& constraint);
	void layout(const geom::Box& size);
	virtual string toString() const = 0;
	void loadXML(element::ElementFactory* const factory, tinyxml2::XMLElement* const element);
	weak_ptr<Element> getElementById(const std::string& id);
	weak_ptr<Element> getElementByPoint(const geom::Vector& screenPoint);
	virtual void idle(const float delta_ms);
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) = 0;
	virtual geom::Box onMeasure(const geom::Box& constraint) = 0;
	virtual void onLayout(const geom::Box& size) = 0;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual weak_ptr<Element> getElementByIdImpl(const std::string& id) = 0;
protected:
	Element(logging::Logger& log, weak_ptr<World> world, weak_ptr<Element> root, weak_ptr<Element> parent);
private:
	void init(weak_ptr<Element> _self);
public:
	template <typename SubKlass>
	static shared_ptr<SubKlass> create(logging::Logger& log, weak_ptr<World> world, weak_ptr<Element> root, weak_ptr<Element> parent)
	{
		shared_ptr<SubKlass> ptr(new SubKlass(log, world, root, parent));
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
