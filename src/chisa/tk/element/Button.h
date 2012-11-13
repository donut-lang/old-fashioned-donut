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
#include "LeafElement.h"
#include "../../gl/Drawable.h"

namespace chisa {
namespace tk {
namespace element {

class Button: public LeafElement {
	CHISA_ELEMENT_SUBKLASS(Button);
private:
	std::string text_;
	Handler<gl::TextDrawable> textImage_;
	bool vertical_;
private:
	geom::Box renderOffset_;
	int pushedCnt_;
private:
	void text(const std::string& text);
	std::string text() const{ return this->text_; };
private:
	Handler<gl::TextDrawable> textImage();
	void onClick();
public:
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInScreen) override;
};

}}}
