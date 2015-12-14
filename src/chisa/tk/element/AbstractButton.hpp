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

class AbstractButton: public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(AbstractButton);
public:
	struct AttrName{
		const static std::string Text;
		const static std::string TextSize;
	};
private: /* 設定されうるリソース */
	std::string text_;
	float textSize_;
	bool vertical_;
private: /* 描画のための一時的なリソース */
	Handler<gl::TextDrawable> textImage_;
	int pushedCnt_;
public:
	void text(std::string const& text);
	void textSize(float const& size);
	void setVertical(bool isVertical);
public:
	inline std::string text() const noexcept{ return this->text_; };
	inline float textSize() const noexcept { return this->textSize_; };
	inline bool isVertical() const noexcept { return this->vertical_; };
protected:
	virtual std::string toString() const override;
	int pushedCount() const noexcept { return this->pushedCnt_; };
	Handler<gl::TextDrawable> textImage();
protected:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
protected: /* 子クラスの実装すべきメソッド */
	virtual geom::Box measureButtonContent(geom::Box const& constraint) = 0;
	virtual void layoutButtonContent(geom::Box const& size) = 0;
	virtual void renderOn(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) = 0;
	virtual void renderOff(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) = 0;
	virtual void renderDisabled(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) = 0;
	virtual bool isOn() const noexcept = 0;
	virtual void onClick();
	virtual void enabledImpl(bool const& newstate);
public:
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override;
};

}}
