/*
 * ScrollLayout.h
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#pragma once

#include "../Layout.h"
#include "../Task.h"
#include "LayoutFactory.h"

namespace chisa {
namespace tk {
namespace layout {

class ScrollLayout: public chisa::tk::Layout {
	CHISA_LAYOUT_SUBKLASS_FINAL(ScrollLayout);
private:
	static constexpr float ScrollBarTimeOut = 500.0f;
	enum Mode {
		None = 0x00,
		Vertical = 0x01,
		Horizontal = 0x10,
		Both = 0x11
	};
	enum Mode scrollMode_;
	geom::Point scrollOffset_;
	shared_ptr<Layout> child_;
	geom::Box childSize_;
	float lastMovedFrom_;
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
	virtual string toString() const override;
	virtual void idle(const float delta_ms) override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual weak_ptr<Layout> getLayoutByIdImpl(const std::string& id) override;
public:
	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance) override;
};

}}}
