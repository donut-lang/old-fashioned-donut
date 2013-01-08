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

namespace chisa {
namespace tk {
class Widget;

class WidgetElement: public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(WidgetElement);
public:
	enum FitMode {
		Center,
		Fit,
		Expand
	};
private:
	WidgetElement* borrowed_;
	DEFINE_MEMBER(public, private, Widget*, widget);
	std::string widgetId_;
	FitMode fitMode_;
	DEFINE_MEMBER(private, private, geom::ScaleVector, widgetScale);
	DEFINE_MEMBER(private, private, geom::Point, widgetOffset);
	DEFINE_MEMBER(private, private, geom::Box, widgetSize);
	DEFINE_MEMBER(private, private, geom::Box, widgetSizeReal);
	DEFINE_MEMBER(private, private, geom::Area, widgetDrawnArea);
public:
	virtual void idle(const float delta_ms) override;
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInScreen, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
private:
	float calcScale(geom::Box const& widget, geom::Box const& constraint);
private:
	geom::Point calcPtInWidget(geom::Point const& ptInScreen);
	virtual void notifyRelayoutFinished();
public:
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity) override;
	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override;
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override;

};

}}
