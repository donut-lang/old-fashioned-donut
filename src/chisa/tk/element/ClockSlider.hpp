/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2015, PSI
 */

#pragma once

#include "../Element.hpp"
#include "../../gl/Drawable.hpp"

namespace chisa {
namespace tk {

class ClockSlider: public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(ClockSlider);
public:
	struct AttrName{
		const static std::string Max;
		const static std::string Min;
		const static std::string Current;
		const static std::string DonutMachineName;
	};
private: /* 設定されうるリソース */
	int max_;
	int min_;
	int current_;
private:
	std::string machineName_;
	XValue message_;
public:
	void max(int const& max);
	void min(int const& min);
	void current(int const& current);
public:
	inline int max() const noexcept{ return this->max_; };
	inline int min() const noexcept { return this->min_; };
	inline int current() const noexcept { return this->current_; };
protected:
	virtual std::string toString() const override;
protected:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override final;
};

}}
