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
#include "Gesture.h"
#include <tarte/ClassUtil.h>
#include <tarte/Handler.h>
#include <tarte/VectorMap.h>
#include <tarte/XMLAttrParser.h>
#include "../geom/Vector.h"
#include "../geom/Area.h"
#include "../gl/Color.h"
#include <string>
#include <cmath>

namespace tinyxml2 {
class XMLElement;
}

namespace chisa {
using namespace tarte;

namespace gl {
class Canvas;
}

namespace tk {
class ElementFactory;

class World;

class Element : public HandlerBody<Element>, public GestureListener {
public:
	struct AttrName {
		const static std::string Id;
		const static std::string Padding;
		const static std::string Margin;
		const static std::string EdgeWidth;
		const static std::string EdgeColor;
		const static std::string ForegroundColor;
		const static std::string BackgroundColor;
	};
private:
	DISABLE_COPY_AND_ASSIGN(Element);
private: /* クラス固定 */
	DEFINE_MEMBER_REF(protected, Logger, log); //ロガー
	VectorMap<std::string, std::function<void(tinyxml2::XMLElement*)> > attrMap_; //コンストラクタでセット
private: /* ツリー */
	DEFINE_MEMBER(protected, private, HandlerW<World>, world); // 属する世界
	DEFINE_MEMBER(public, private, HandlerW<Element>, parent); //親
	DEFINE_MEMBER(public, private, std::string, id); //要素に付けられたID
	geom::Space margin_;
	geom::Space padding_;
	gl::Color edgeColor_;
	float edgeWidth_;
	gl::Color foregroundColor_;
	gl::Color backgroundColor_;
	bool relayoutRequested_;
private: /* 画面描画情報 */
	DEFINE_MEMBER(public, private, geom::Box, size); //現在の大きさ
	DEFINE_MEMBER(public, private, geom::Area, screenArea); //画面上の占める位置
	DEFINE_MEMBER(protected, private, geom::Area, drawnArea); //大きさの中で、レンダリングされている部分
	bool onFocused_;
public:
	void margin(geom::Space const& m);
	void padding(geom::Space const& p);
	void foregroundColor(gl::Color const& c);
	void backgroundColor(gl::Color const& c);
	void edgeColor(gl::Color const& c);
	void edgeWidth(float const& f);
	inline geom::Space const& margin() noexcept { return this->margin_; };
	inline geom::Space const& padding() const noexcept { return this->padding_; };
	inline gl::Color const& foregroundColor() const noexcept { return this->foregroundColor_; };
	inline gl::Color const& backgroundColor() const noexcept { return this->backgroundColor_; };
	inline bool onFocused() const noexcept { return this->onFocused_; };
public: /* レンダリング(非virtual) */
	void render(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area);
	geom::Box measure(geom::Box const& constraint);
	void layout(geom::Box const& size);
	virtual bool isValidationRoot() const noexcept;
	void requestRelayout();
	virtual void notifyRelayoutFinished();
	void notifyViewRefreshed();
private:
	void forceRelayout();
public: /* ツリー操作 */
	Handler<Element> findRootElement();
	virtual Handler<Element> findElementById(std::string const& id);
	virtual Handler<Element> findElementByPoint(geom::Vector const& screenPoint);
public: /* 木の生成 */
	void loadXml(ElementFactory* const factory, tinyxml2::XMLElement* const element);
public: /* バックグラウンドタスク */
	virtual void idle(const float delta_ms);
public: /* 実装メソッド */
	virtual std::string toString() const;
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) = 0;
	virtual geom::Box measureImpl(geom::Box const& constraint) = 0;
	virtual void layoutImpl(geom::Box const& size) = 0;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual bool notifyViewRefreshedImpl();
	virtual void onFocusGained(const float timeMs) override;
	virtual void onFocusLost(const float timeMs) override;
protected:
	Element(Logger& log, HandlerW<World> world, HandlerW<Element> parent);
	template <typename T> void addAttribute(std::string const& name, T& ptr)
	{
		this->attrMap_.insert(name, std::bind(xml::parseAttr<T>, std::string(name), std::ref(ptr), std::ref(ptr), std::placeholders::_1));
	}
	template <typename T> Handler<T> createChild(){
		return Handler<T>(new T(this->log(), this->world(), this->self()));
	}
public:
	inline bool onFree() noexcept { return false; };
	virtual ~Element() noexcept = default;
};


#define CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST Logger& log, HandlerW<World> world, HandlerW<Element> parent
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
