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
#include "ElementGroup.h"
#include "../../util/VectorMap.h"
#include "../../geom/Area.h"

namespace chisa {
namespace tk {
namespace element {

class FrameCombo;
class SplitCombo;
class Button;
class TabCombo : public ElementGroup {
	CHISA_ELEMENT_SUBKLASS(TabCombo);
public:
	enum ButtonPosition{
		Top,
		Left,
		Bottom,
		Right
	};
private:
	Handler<SplitCombo> top_;
	Handler<FrameCombo> frame_;
	Handler<SplitCombo> buttons_;
	ButtonPosition buttonPosition_;
	util::VectorMap<Handler<Element>, Handler<Button> > buttonMap_;
public: /* ツリー操作 */
	virtual std::size_t getChildCount() const noexcept override final;
	virtual Handler<Element> getChildAt( std::size_t const& idx ) const noexcept override final;
	virtual void addChild(Handler<Element> const& h) override final;
	virtual void addChild(std::size_t const& idx, Handler<Element> const& h) override final;
	virtual Handler<Element> removeChild(std::size_t const& idx) override final;
	virtual Handler<Element> removeChild(Handler<Element> const& h) override final;
	virtual Handler<Element> lastChild() const noexcept override final;
	virtual Handler<Element> frontChild() const noexcept override final;
	virtual std::size_t bringChildToLast(Handler<Element> const& e) override final;
	virtual std::size_t bringChildToFront(Handler<Element> const& e) override final;
public: /* ツリー操作 */
	virtual Handler<Element> findElementById(std::string const& id) override final;
	virtual Handler<Element> findElementByPoint(geom::Vector const& screenPoint) override final;
public: /* バックグラウンドタスク */
	virtual void idle(const float delta_ms) override final;
public: /* 実装メソッド */
	virtual std::string toString() const override final;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
};

}}}
