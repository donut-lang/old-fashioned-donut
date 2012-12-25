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

class TextArea : public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(TextArea);
public:
	struct AttrName{
		const static std::string Description;
		const static std::string TextSize;
	};
private: /* 設定されうるリソース */
	float textSize_;
	std::string description_;
private: /* 変化するリソース */
	std::string text_;
private: /* 描画のための一時的なリソース */
	Handler<gl::TextDrawable> textImage_;
	Handler<gl::TextDrawable> descImage_;
	std::vector<Handler<gl::TextDrawable> > editState_;
	std::size_t editPos_;
public:
	void text(std::string const& text);
	void description(std::string const& text);
	void textSize(float const& size);
public:
	inline std::string text() const noexcept{ return this->text_; };
	inline std::string description() const noexcept{ return this->description_; };
	inline float textSize() const noexcept { return this->textSize_; };
private:
	virtual std::string toString() const override final;
	Handler<gl::TextDrawable> textImage();
	Handler<gl::TextDrawable> descImage();
	void onClick();
	void startEditing();
	void stopEditing();
protected:
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override final;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
public:
	virtual bool onMoveRaw(const float timeMs, geom::Point const& ptInScreen) override;
	virtual bool onSingleTapUp(const float timeMs, geom::Point const& ptInScreen) override;
	virtual void onFocusGained(const float timeMs) override;
	virtual void onFocusLost(const float timeMs) override;
};

}}
