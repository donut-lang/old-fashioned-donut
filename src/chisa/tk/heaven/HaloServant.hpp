/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Angel.hpp"
#include "../../gl/Color.hpp"

namespace chisa {
namespace tk {

class HaloServant : public Servant {
private:
	gl::Color color_;
public:
	inline gl::Color const& color() const { return this->color_; };
public:
	HaloServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color);
	virtual ~HaloServant() noexcept = default;
public:
	Handler<HaloServant> self() noexcept;
private:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
};

}}
