/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Chisa_WIDGETWRAPPERLAYOUT_H__CPP_
#define Chisa_WIDGETWRAPPERLAYOUT_H__CPP_

#include "../Layout.h"
#include <memory>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace layout {
class LayoutFactory;
using namespace std;
using namespace tinyxml2;

class WidgetWrapperLayout: public chisa::tk::Layout {
	CHISA_LAYOUT_SUBKLASS_FINAL(WidgetWrapperLayout);
public:
	enum FitMode {
		Center,
		Fit
	};
private:
	WidgetWrapperLayout* parent_;
	DEFINE_MEMBER(private, private, Widget*, widget);
	std::string widgetId_;
	FitMode fitMode_;
	DEFINE_MEMBER(private, private, float, widgetScale);
	DEFINE_MEMBER(private, private, Point, widgetPoint);
	DEFINE_MEMBER(private, private, Box, widgetSize);
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
public:
	virtual void renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area) override;
	virtual Box onMeasure(const Box& constraint) override;
	virtual void onLayout(const Box& size) override;
	virtual void idle(const float delta_ms) override;
public:
	virtual void loadXML(LayoutFactory* const factory, XMLElement* const element) override;
	virtual string toString() override;
public:
	virtual void calcPositionOf();
};

}}}
#endif /* INCLUDE_GUARD */
