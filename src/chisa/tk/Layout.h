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

#ifndef Chisa_TK_LAYOUT_H__CPP_
#define Chisa_TK_LAYOUT_H__CPP_

#include "Widget.h"
#include "../util/class_utils.h"
#include <memory>
#include <string>
#include <cmath>

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
namespace tk {
namespace layout {
class LayoutFactory;
}

class World;
using std::string;
using std::weak_ptr;
using std::size_t;

class Layout {
	DISABLE_COPY_AND_ASSIGN(Layout);
private:
	DEFINE_MEMBER_REF(protected, logging::Logger, log);
	DEFINE_MEMBER(protected, private, weak_ptr<World>, world);
	DEFINE_MEMBER(protected, private, weak_ptr<Layout>, root);
	DEFINE_MEMBER(protected, private, weak_ptr<Layout>, parent);
	DEFINE_MEMBER(protected, private, weak_ptr<Layout>, self);
	DEFINE_MEMBER(public, private, Box, size);
	DEFINE_MEMBER(protected, private, Area, screenArea);
	DEFINE_MEMBER(protected, private, Area, drawnArea);
	DEFINE_MEMBER(public, private, std::string, id);
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const = 0;
	virtual size_t getChildCount() const = 0;
public:
	void render(gl::Canvas& canvas, const Area& screenArea, const Area& area);
	Box measure(const Box& constraint);
	void layout(const Box& size);
private:
	virtual void renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area) = 0;
	virtual Box onMeasure(const Box& constraint) = 0;
	virtual void onLayout(const Box& size) = 0;
	virtual void loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual weak_ptr<Layout> getLayoutByIdImpl(const std::string& id) = 0;
protected:
	Layout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
private:
	void init(weak_ptr<Layout> _self);
public:
	template <typename SubKlass>
	static shared_ptr<SubKlass> create(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
	{
		shared_ptr<SubKlass> ptr(new SubKlass(log, world, root, parent));
		ptr->init(ptr);
		return ptr;
	}
	virtual ~Layout();
	virtual string toString() = 0;
	void loadXML(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element);
	weak_ptr<Layout> getLayoutById(const std::string& id);
	virtual void idle(const float delta_ms);
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


#define CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent
#define CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY log, world, root, parent

#define CHISA_LAYOUT_SUBKLASS_FINAL(Klass) \
friend shared_ptr<Klass> Layout::create<Klass>(CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
private:\
	Klass(CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
public:\
	virtual ~Klass();

#define CHISA_LAYOUT_SUBKLASS(Klass) \
protected:\
	Klass(CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
public:\
	virtual ~Klass();

#define CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived) Derived(CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY)

#define CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Derived) \
Klass::Klass(CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)\
:CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)

#define CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(Klass) CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Layout)

}}
#endif /* INCLUDE_GUARD */
