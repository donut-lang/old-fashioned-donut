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
	CHISA_ELEMENT_SUBKLASS_FINAL(Button);
public:
	struct AttrName{
		const static std::string Text;
		const static std::string TextSize;
		const static std::string ForegroundColor;
		const static std::string BackgroundColor;
		const static std::string ShadowColor;
		const static std::string ShadowDepth;
	};
private:
	std::string text_;
	float textSize_;
	geom::Space margin_;
	geom::Space padding_;
	bool vertical_;
	gl::Color foregroundColor_;
	gl::Color backgroundColor_;
	gl::Color shadowColor_;
	float shadowDepth_;
private:
	Handler<gl::TextDrawable> textImage_;
private:
	geom::Box renderOffset_;
	int pushedCnt_;
private:
	void text(std::string const& text);
	std::string text() const{ return this->text_; };
private:
	Handler<gl::TextDrawable> textImage();
	void onClick();
public:
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual geom::Box onMeasure(geom::Box const& constraint) override;
	virtual void onLayout(geom::Box const& size) override;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
public:
	virtual bool onDownRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onUpRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onMoveRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, geom::Point const& ptInScreen) override;
};

}}}
