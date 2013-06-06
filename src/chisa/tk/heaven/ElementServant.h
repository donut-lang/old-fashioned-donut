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

class ElementServant : public Servant {
private:
	Handler<Element> element_;
	float anim_;
	bool animEnabled_;
public:
	inline Handler<Element> const& element() const { return this->element_; };
public:
	Handler<ElementServant> self() noexcept;
	inline bool const& animEnabled() const noexcept { return this->animEnabled_; };
	inline void animEnabled(bool const& enabled) noexcept { this->animEnabled_ = enabled; };
public:
	ElementServant(Handler<AngelTarget> const& angelTarget, Handler<Element> const& elem);
	virtual ~ElementServant() noexcept = default;
private:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void onShownImpl() override final;
	virtual void onHiddenImpl() override final;
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
private:
	virtual Handler<Element> findElementById(std::string const& id) override final;
	virtual Handler<Element> findElementByPoint(geom::Point const& screenVector) override final;
};


}}
