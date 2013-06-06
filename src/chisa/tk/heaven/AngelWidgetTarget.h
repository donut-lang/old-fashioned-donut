/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Angel.h"

namespace chisa {
namespace tk {

class WidgetElement;
class AngelWidgetTarget : public AngelTarget {
public:
	AngelWidgetTarget(Handler<Angel> const& angel, std::string const& id, std::string const& guide);
	virtual ~AngelWidgetTarget() noexcept = default;
private:
	std::string const id_;
	HandlerW<Widget> widget_;
	std::string const guide_;
public:
	Handler<AngelWidgetTarget> self() noexcept;
	Handler<WidgetElement> findElement();
	Handler<Widget> findWidget();
private:
	virtual geom::Area findScreenAreaImpl();
	virtual Handler<AngelWidgetTarget> matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept override final;
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void onAttatchedImpl() override final;
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
	virtual Handler<AngelWidgetTarget> toWidgetTarget() override final;
};

}}
