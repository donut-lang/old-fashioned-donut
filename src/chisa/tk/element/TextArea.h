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

#include <deque>
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
public:
	inline std::string text() const noexcept{ return this->text_; };
	inline std::string description() const noexcept{ return this->description_; };
	inline float textSize() const noexcept { return this->textSize_; };
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
	virtual void renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override final;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual bool notifyViewRefreshedImpl() override;
public:
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
