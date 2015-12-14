/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ElementGroup.hpp"
#include "../Element.hpp"

namespace chisa {
namespace tk {

class FrameCombo : public ElementGroupBase<bool> {
	CHISA_ELEMENT_SUBKLASS(FrameCombo);
private:
	std::size_t numShown_;
public:
	virtual std::string toString() const override;
public:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
};

}}
