/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ElementGroup.hpp"
#include "../../geom/Area.hpp"

namespace chisa {
namespace tk {

class MarginCombo : public ElementGroupBase<std::nullptr_t> {
	CHISA_ELEMENT_SUBKLASS(MarginCombo);
	geom::Space margin_;
public:
	virtual std::string toString() const override;
private:
	using Super::addChild;
	virtual void addChild(Handler<Element> const& h, Context const& ctx) override final;
	virtual void addChild(std::size_t const& idx, Handler<Element> const& h, Context const& ctx) override final;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
};

}}
