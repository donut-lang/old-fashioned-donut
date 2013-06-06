/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../Element.h"

namespace chisa {
namespace tk {
class Widget;

class WidgetElement final : public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(WidgetElement);
public:
	enum FitMode {
		Center,
		Fit,
		Expand
	};
private:
	HandlerW<WidgetElement> borrowed_;
	Handler<Widget> widget_;
	std::string widgetId_;
	FitMode fitMode_;
	DEFINE_MEMBER(private, private, geom::ScaleVector, widgetScale);
	DEFINE_MEMBER(private, private, geom::Point, widgetOffset);
	DEFINE_MEMBER(private, private, geom::Box, widgetSize);
	DEFINE_MEMBER(private, private, geom::Box, widgetSizeReal);
	DEFINE_MEMBER(private, private, geom::Area, widgetDrawnArea);
public:
	virtual void idle(const float delta_ms) override;
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInScreen, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
private:
	float calcScale(geom::Box const& widget, geom::Box const& constraint);
private:
	virtual void notifyRelayoutFinished();
public:
	inline Handler<Widget> const& widget() const noexcept { return this->widget_; };
	geom::Point calcPtInWidgetRel(geom::Point const& ptInRoot);
	geom::Point calcPtInWidgetAbs(geom::Point const& ptInRoot);
	geom::Point calcPtInElement(geom::Point const& ptInWidgetAbs);
	geom::Point calcPtInRoot(geom::Point const& ptInWidgetAbs);

	geom::Area calcAreaInElement(geom::Area const& areaInWidget);
	geom::Area calcAreaInRoot(geom::Area const& areaInWidget);
public:
	inline Handler<WidgetElement> self() { return Handler<WidgetElement>::__internal__fromRawPointerWithoutCheck(this); };
public:
	virtual void onShownImpl() override final;
	virtual void onHiddenImpl() override final;
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity) override final;
	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override final;
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override final;
	virtual void onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen) override;
	virtual void onFocusLost(float const& timeMs) override;
	virtual bool onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym) override final;
	virtual bool onKeyUp(float const& timeMs, SDL_Keysym const& sym) override final;
};

}}
