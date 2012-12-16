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
#include "../util/VectorMap.h"
#include "../util/XMLAttrParser.h"
#include "../geom/Vector.h"
#include "../geom/Area.h"
#include "../Handler.h"
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

class Element : public HandlerBody<Element>, public GestureListener {
	DISABLE_COPY_AND_ASSIGN(Element);
private: /* クラス固定 */
	DEFINE_MEMBER_REF(protected, logging::Logger, log); //ロガー
	util::VectorMap<std::string, std::function<void(tinyxml2::XMLElement*)> > attrMap_; //コンストラクタでセット
public: /* ツリー */
	DEFINE_MEMBER(protected, private, HandlerW<World>, world); // 属する世界
	DEFINE_MEMBER(public, private, HandlerW<Element>, root); //Rootエレメント
	DEFINE_MEMBER(public, private, HandlerW<Element>, parent); //親
	DEFINE_MEMBER(public, private, std::string, id); //要素に付けられたID
public: /* 画面描画情報 */
	DEFINE_MEMBER(public, private, geom::Box, size); //現在の大きさ
	DEFINE_MEMBER(public, private, geom::Area, screenArea); //画面上の占める位置
	DEFINE_MEMBER(protected, private, geom::Area, drawnArea); //大きさの中で、レンダリングされている部分
public: /* レンダリング */
	void render(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area);
	geom::Box measure(geom::Box const& constraint);
	void layout(geom::Box const& size);
public: /* ツリー操作 */
	Handler<Element> findRootElement();
	Handler<Element> getElementById(std::string const& id);
	Handler<Element> getElementByPoint(geom::Vector const& screenPoint);
	virtual Handler<Element> getChildAt(const size_t index) const = 0;
	virtual size_t getChildCount() const = 0;
public: /* 木の生成 */
	void loadXML(element::ElementFactory* const factory, tinyxml2::XMLElement* const element);
public: /* バックグラウンドタスク */
	virtual void idle(const float delta_ms);
public: /* 実装メソッド */
	virtual std::string toString() const = 0;
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) = 0;
	virtual geom::Box onMeasure(geom::Box const& constraint) = 0;
	virtual void onLayout(geom::Box const& size) = 0;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual Handler<Element> getElementByIdImpl(std::string const& id) = 0;
protected:
	Element(logging::Logger& log, HandlerW<World> world, HandlerW<Element> parent);
	template <typename T> void addAttribute(std::string const& name, T& ptr)
	{
		this->attrMap_.insert(name, std::bind(chisa::util::xml::parseAttr<T>, std::string(name), std::ref(ptr), std::ref(ptr), std::placeholders::_1));
	}
public:
	inline bool onFree() noexcept { return false; };
	virtual ~Element() noexcept = default;
};


#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST logging::Logger& log, HandlerW<World> world, HandlerW<Element> parent
#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY log, world, parent

#define CHISA_ELEMENT_SUBKLASS_FINAL(Klass) \
public:\
	Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
	virtual ~Klass() noexcept;

#define CHISA_ELEMENT_SUBKLASS(Klass) \
public:\
	Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);\
	virtual ~Klass() noexcept;

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)\
Derived(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY)

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Derived) \
Klass::Klass(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)\
:CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_SETUP_BASE(Derived)

#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(Klass) CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Klass, Element)

}}
