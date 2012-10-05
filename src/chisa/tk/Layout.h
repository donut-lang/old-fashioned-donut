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
	logging::Logger& log_;
	weak_ptr<World> world_;
	weak_ptr<Layout> root_;
	weak_ptr<Layout> parent_;
	weak_ptr<Layout> self_;
	Area area_;
protected:
	inline logging::Logger& log() const { return log_; }
	inline weak_ptr<World> world() const { return world_; }
	inline weak_ptr<Layout> root() const { return root_; }
	inline weak_ptr<Layout> parent() const { return parent_; }
	inline weak_ptr<Layout> self() const { return self_; }
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const = 0;
	virtual size_t getChildCount() const = 0;
	virtual weak_ptr<Widget> getWidgetById(const string& id) = 0;
public:
	Area area() const {return area_;};
public:
	/**
	 * 描画する。
	 * エリアは描画される、レイアウト内の位置。
	 * area ⊂ this->area.box();
	 */
	virtual void render(const Area& area) = 0; /* OpenGLの座標の設定などを行なってしまう */
	virtual void idle(const float delta_ms) = 0;
	virtual Box measure(const Box& constraint) = 0;
	virtual void loadXML(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) = 0;
	virtual string toString() = 0;
	/**
	 * ウインドウサイズの変更などが起こったので、レイアウトの再計算を行う。
	 * ここのエリアはウインドウ全体内での位置。
	 */
	void reshape(const Area& area);
protected:
	/**
	 * サブクラスはこっちを実装する。
	 */
	virtual void reshapeImpl(const Area& area) = 0;
public:
	Layout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
	virtual ~Layout();
public:
	void init(weak_ptr<Layout> _self);
};

}}
#endif /* INCLUDE_GUARD */
