/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Element.h"
#include "AbstractButton.h"

namespace chisa {
namespace tk {

class ClickButton: public AbstractButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(ClickButton);
public:
	struct AttrName{
		const static std::string ShadowColor;
		const static std::string ShadowDepth;
	};
private:
	gl::Color shadowColor_;
	float shadowDepth_;
	geom::Box renderOffset_;
public:
	virtual std::string toString() const override;
	inline gl::Color const& shadowColor() const noexcept { return this->shadowColor_; };
	inline float shadowDepth() const noexcept { return this->shadowDepth_; };
	void shadowColor(gl::Color const& color);
	void shadowDepth(float const& depth);
private:
	virtual geom::Box measureButtonContent(geom::Box const& constraint) override final;
	virtual void layoutButtonContent(geom::Box const& size) override final;
	virtual void renderOn(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual void renderOff(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual void renderDisabled(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual bool isOn() const noexcept override final;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual void onClick() override = 0;
};

}}
