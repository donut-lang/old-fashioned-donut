/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Element.hpp"
#include "../../gl/Drawable.hpp"

namespace chisa {
namespace tk {

class Label: public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(Label);
public:
	struct AttrName{
		const static std::string Text;
		const static std::string FontSize;
		const static std::string DonutMachineName;
	};
private: /* 設定されうるリソース */
	std::string text_;
	float fontSize_;
	bool vertical_;
private:
	std::string machineName_;
	XValue message_;
private: /* 描画のための一時的なリソース */
	Handler<gl::TextDrawable> textImage_;
public:
	void text(std::string const& text);
	void textSize(float const& size);
	void setVertical(bool isVertical);
public:
	inline std::string text() const noexcept{ return this->text_; };
	inline float textSize() const noexcept { return this->fontSize_; };
	inline bool isVertical() const noexcept { return this->vertical_; };
protected:
	virtual std::string toString() const override;
	Handler<gl::TextDrawable> textImage();
protected:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override final;
};

}}
