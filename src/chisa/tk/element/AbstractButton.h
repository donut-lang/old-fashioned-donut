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
#include "../../gl/Drawable.h"

namespace chisa {
namespace tk {

class AbstractButton: public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(AbstractButton);
public:
	struct AttrName{
		const static std::string Text;
		const static std::string TextSize;
		const static std::string EdgeColor;
		const static std::string ShadowColor;
		const static std::string ShadowDepth;
	};
private:
	std::string text_;
	float textSize_;
	bool vertical_;
	gl::Color edgeColor_;
	gl::Color shadowColor_;
	float shadowDepth_;
private:
	Handler<gl::TextDrawable> textImage_;
private:
	geom::Box renderOffset_;
	int pushedCnt_;
public:
	void text(std::string const& text);
	void textSize(float const& size);
	void setVertical(bool isVertical);
	void shadowColor(gl::Color const& color);
	void shadowDepth(float const& depth);
public:
	inline std::string text() const noexcept{ return this->text_; };
	inline float textSize() const noexcept { return this->textSize_; };
	inline bool isVertical() const noexcept { return this->vertical_; };
	inline gl::Color const& shadowColor() const noexcept { return this->shadowColor_; };
	inline float shadowDepth() const noexcept { return this->shadowDepth_; };
private:
	Handler<gl::TextDrawable> textImage();
	void onClick();
public:
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
public:
	virtual bool onDownRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onUpRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onMoveRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, geom::Point const& ptInScreen) override;
};

}}
