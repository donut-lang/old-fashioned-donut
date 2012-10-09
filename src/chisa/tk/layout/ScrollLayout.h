/*
 * ScrollLayout.h
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#ifndef __CXX_ChisaSCROLLLAYOUT_H_
#define __CXX_ChisaSCROLLLAYOUT_H_

#include "../Layout.h"
#include "LayoutFactory.h"

namespace chisa {
namespace tk {
namespace layout {

class ScrollLayout: public chisa::tk::Layout {
	CHISA_LAYOUT_SUBKLASS_FINAL(ScrollLayout);
private:
	enum Mode {
		None = 0x00,
		Vertical = 0x01,
		Horizontal = 0x10,
		Both = 0x11
	};
	enum Mode scrollMode_;
	Point nowPosition_;
	shared_ptr<Layout> child_;
	Box childSize_;
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
	virtual void loadXML(LayoutFactory* const factory, tinyxml2::XMLElement* element) override;
	virtual string toString() override;
public:
	virtual void renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area) override;
	virtual Box onMeasure(const Box& constraint) override;
	virtual void onLayout(const Box& size) override;
};

}}}
#endif /* INCUDE_GUARD */
