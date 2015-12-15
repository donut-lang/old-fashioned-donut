/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../Angel.hpp"

namespace chisa {
namespace tk {

class LoneAngel final : public Angel {
public:
	LoneAngel(Handler<Heaven> const& heaven);
	virtual ~LoneAngel() noexcept = default;
public:
	Handler<LoneAngel> self() noexcept;
protected:
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual void reshapeImpl(geom::Area const& area) override final;
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
public:
	virtual Handler<AngelTarget> attatchTarget(Handler<AngelTarget> const& target) override final;
	virtual Handler<AngelTarget> detatchTarget(Handler<AngelTarget> const& target) override final;
};

}}