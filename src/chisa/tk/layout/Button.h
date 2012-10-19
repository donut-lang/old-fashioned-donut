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

#include "../Layout.h"
#include "../../gl/RawSprite.h"

namespace chisa {
namespace tk {
namespace widget {

class Button: public chisa::tk::Layout {
	CHISA_LAYOUT_SUBKLASS(Button);
private:
	cairo_surface_t* const surface_;
	cairo_t* const cairo_;
	std::string text_;
	geom::Box textSize_;
	geom::Vector textOffset_;
	gl::Handler<gl::RawSprite> sprite_;
	bool dirty_;
	bool vertical_;
private:
	geom::Box renderOffset_;
	int pushedCnt_;
private:
	void text(const std::string& text);
	std::string text() const{ return this->text_; };
private:
	void realizeText(gl::Canvas& canvas);
	void measureTextSize();
	void onClick();
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
	virtual string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual weak_ptr<Layout> getLayoutByIdImpl(const std::string& id) override;
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInScreen) override;
};

}}}
