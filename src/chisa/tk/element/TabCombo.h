/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/VectorMap.h>

#include "ElementGroup.h"
#include "../../geom/Area.h"

namespace chisa {
namespace tk {

class FrameCombo;
class SplitCombo;
class TabButton;
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
	VectorMap<Handler<Element>, Handler<TabButton> > buttonMap_;
public: /* ツリー操作 */
	virtual void onShown() override final;
	virtual void onHidden() override final;
	virtual std::size_t getChildCount() const noexcept override final;
	virtual Handler<Element> getChildAt( std::size_t const& idx ) const noexcept override final;
	virtual void addChild(Handler<Element> const& h) override final;
	virtual void addChild(std::size_t const& idx, Handler<Element> const& h) override final;
	void addChild(Handler<Element> const& h, std::string const& title);
	void addChild(std::size_t const& idx, Handler<Element> const& h, std::string const& title);
	virtual Handler<Element> removeChild(std::size_t const& idx) override final;
	virtual Handler<Element> removeChild(Handler<Element> const& h) override final;
	virtual Handler<Element> lastChild() const noexcept override final;
	virtual Handler<Element> frontChild() const noexcept override final;
	virtual std::size_t bringChildToLast(Handler<Element> const& e) override final;
	virtual std::size_t bringChildToFront(Handler<Element> const& e) override final;
public: /* ツリー操作 */
	virtual Handler<Element> findElementById(std::string const& id) override final;
	virtual Handler<Element> findElementByPoint(geom::Vector const& screenPoint) override final;
	virtual void notifyRelayoutFinished() override final;
public: /* バックグラウンドタスク */
	virtual void idle(const float delta_ms) override final;
public: /* 実装メソッド */
	virtual std::string toString() const override final;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
private:
	void updateButtonState(Handler<Element> const& elm, Handler<TabButton> const& btn);
};

}}
