/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <deque>
#include "../Element.hpp"
#include "../../gl/Drawable.hpp"

namespace chisa {
namespace tk {

class TextArea : public Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(TextArea);
public:
	struct AttrName{
		const static std::string Description;
		const static std::string TextSize;
		const static std::string FrameColor;
	};
private: /* 設定されうるリソース */
	float textSize_;
	std::string description_;
	gl::Color frameColor_;
private: /* 変化するリソース */
	std::string text_;
private: /* 描画のための一時的なリソース */
	bool editing_;
	Handler<gl::TextDrawable> textImage_;
	Handler<gl::TextDrawable> descImage_;
private:
	std::vector<Handler<gl::TextDrawable> > editListBefore_; //カーソル前の文字列
	std::vector<Handler<gl::TextDrawable> > editListEditing_; //今まさにIMEで入力している文字列バッファ
	geom::Area textArea_; //最後表示した時のテキストエリア
	float textOffset_; //テキストが表示エリアをはみ出した時のためのオフセット
	float editListBeforeWidth_; //カーソル前のテキストの長さ
	float editListAfterWidth_; //カーソル後のテキストの長さ
	int editLength_; //カーソルから何文字分セレクトしているか。マイナスもあり得るよ
	float editWidth_; //セレクトしてる分のテキストの長さ
	std::deque<Handler<gl::TextDrawable> > editListAfter_; //カーソル後の文字列
	float cursorTimer_; //カーソルの表示用タイマ
public:
	void text(std::string const& text);
	void description(std::string const& text);
	void textSize(float const& size);
	void frameColor(gl::Color const& c) { this->frameColor_ = c; };
public:
	inline std::string const& text() const noexcept{ return this->text_; };
	inline std::string const& description() const noexcept{ return this->description_; };
	inline float const& textSize() const noexcept { return this->textSize_; };
	inline gl::Color const& frameColor() const noexcept { return this->frameColor_; };
private:
	virtual void idle(const float delta_ms) override;
	virtual std::string toString() const override final;
	Handler<gl::TextDrawable> textImage();
	Handler<gl::TextDrawable> descImage();
	void startEditing(float const width);
	Handler<gl::TextDrawable> createEditingText(Handler<gl::DrawableManager> const& mgr, std::string const& str);
	void stopEditing();

	void startInput();
	void stopInput();

	void startSelection(float width);
	void moveSelection(float width);

	//カーソルの移動、削除
	void fixCursorForward();
	void fixCursorBackward();
	bool moveCursorLeft(bool select);
	bool moveCursorRight(bool select);
	bool moveCursorBegin(bool select);
	bool moveCursorEnd(bool select);
	bool deleteCursorBefore();
	bool deleteCursorAfter();
	bool deleteSelected();
protected:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override final;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
public:
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen) override final;
	virtual void onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen) override final;
	virtual void onFocusLost(float const& timeMs) override final;
	virtual void onTextInput(float const& timeMs, std::string const& text) override final;
	virtual void onTextEdit(float const& timeMs, std::string const& text, int const start, int const length) override final;
	virtual bool onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym) override final;
	virtual bool onKeyUp(float const& timeMs, SDL_Keysym const& sym) override final;
};

}}