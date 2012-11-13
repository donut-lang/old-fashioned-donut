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

#pragma once

#include "../Element.h"
#include <memory>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace element {
class ElementFactory;

class WidgetWrapperLayout: public chisa::tk::Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(WidgetWrapperLayout);
public:
	enum FitMode {
		Center,
		Fit,
		Expand
	};
private:
	WidgetWrapperLayout* borrowed_;
	DEFINE_MEMBER(private, private, Widget*, widget);
	std::string widgetId_;
	FitMode fitMode_;
	DEFINE_MEMBER(private, private, geom::ScaleVector, widgetScale);
	DEFINE_MEMBER(private, private, geom::Point, widgetOffset);
	DEFINE_MEMBER(private, private, geom::Box, widgetSize);
	DEFINE_MEMBER(private, private, geom::Box, widgetSizeReal);
	DEFINE_MEMBER(private, private, geom::Area, widgetDrawnArea);
public:
	virtual std::weak_ptr<Element> getChildAt(const std::size_t index) const override;
	virtual std::size_t getChildCount() const override;
public:
	virtual void idle(const float delta_ms) override;
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual std::weak_ptr<Element> getLayoutByIdImpl(const std::string& id) override;
private:
	float calcScale(const geom::Box& widget, const geom::Box& constraint);
private:
	geom::Point calcPtInWidget(const geom::Point& ptInScreen);
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onMoveRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity) override;
	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance) override;
	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio) override;

};

}}}
