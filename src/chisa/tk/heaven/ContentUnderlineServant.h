/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Angel.h"
#include "../../gl/Color.h"

namespace chisa {
namespace tk {

class ContentUnderlineServant : public Servant {
private:
	gl::Color color_;
	std::string docId_;
public:
	inline gl::Color const& color() const { return this->color_; };
public:
	ContentUnderlineServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color, std::string const& docId);
	virtual ~ContentUnderlineServant() noexcept = default;
public:
	Handler<ContentUnderlineServant> self() noexcept;
private:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
};

}}
