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

class AngelElementTarget : public AngelTarget {
public:
	AngelElementTarget(Handler<Angel> const& angel, std::string const& id);
	virtual ~AngelElementTarget() noexcept = default;
private:
	std::string const id_;
	HandlerW<Element> target_;
public:
	Handler<AngelElementTarget> self() noexcept;
	Handler<Element> findElement();
private:
	virtual geom::Area findScreenAreaImpl();
	virtual Handler<AngelElementTarget> matchToElementTarget(std::string const& elementId) noexcept override final;
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void onAttatchedImpl() override final;
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
	virtual Handler<AngelElementTarget> toElementTarget();
};

}}
