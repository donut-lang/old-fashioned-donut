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

#include <sstream>
#include <tinyxml2.h>
#include <SDL2/SDL.h>
#include <tarte/Exception.h>

#include "TextArea.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

#include "ElementGroup.h"
namespace chisa {
namespace tk {

const std::string TextArea::AttrName::Description("desc");
const std::string TextArea::AttrName::TextSize("text-size");

static constexpr geom::Space TextMargin(3.0f);

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TextArea, Element)
,textSize_(32.0f)
,description_()
,text_()
,textImage_()
,descImage_()
,editing_(false)
,editListBeforeWidth_(0)
,editListAfterWidth_(0)
,editStart_(0)
,editLength_(0)
,cursorCounter_(0)
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->addAttribute(AttrName::Description, this->description_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
}

TextArea::~TextArea() noexcept
{
}

std::string TextArea::toString() const
{
	return ::tarte::format("(TextArea text:\"%s\" %p)", this->text_.c_str(), this);
}

void TextArea::idle(const float delta_ms)
{
	cursorCounter_+=delta_ms;
}

void TextArea::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	canvas.fillRect(gl::White, screenArea);
	geom::Area textArea(TextMargin.apply(screenArea));
	geom::Distance pos(textArea.point());
	if( this->editing_ ) {
		canvas.drawRect(2.0f, gl::DarkYellow, screenArea);
		gl::Color cursorColor(0,0,0,std::abs(std::cos(cursorCounter_/500.0f)));
		if(this->editListEditing_.empty()){
			for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
				d->draw(canvas, pos);
				pos.x(pos.x() + d->width());
			}
			canvas.drawLine(3, cursorColor, pos, pos+geom::Box(0, textArea.height()));
			for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
				d->draw(canvas, pos);
				pos.x(pos.x() + d->width());
			}
		}else{
			for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
				d->draw(canvas, pos);
				pos.x(pos.x() + d->width());
			}
			geom::Point lineStart(pos.x(), pos.y()+textArea.height());
			for(Handler<gl::TextDrawable> const& d : this->editListEditing_) {
				d->draw(canvas, pos);
				pos.x(pos.x() + d->width());
			}
			geom::Point lineEnd(pos.x(), pos.y()+textArea.height());
			canvas.drawLine(1, gl::Black, lineStart, lineEnd);

			canvas.drawLine(3, cursorColor, pos, pos+geom::Box(0, textArea.height()));
			for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
				d->draw(canvas, pos);
				pos.x(pos.x() + d->width());
			}
		}
	}else{
		canvas.drawRect(2.0f, gl::Black, screenArea);
		if(this->text_.empty()) {
			Handler<gl::TextDrawable> spr(this->descImage());
			spr->draw(canvas, textArea.point()+(textArea.box() - spr->size())/2.0f);
		}else{
			Handler<gl::TextDrawable> spr(this->textImage());
			spr->draw(canvas, textArea.point());
		}
	}
}
Handler<gl::TextDrawable> TextArea::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock() ){
			this->textImage_ = w->drawableManager()->queryText(
					this->text(),
					this->textSize_,
					Handler<gl::Font>(),
					gl::TextDrawable::Style::Bold,
					gl::TextDrawable::Decoration::None,
					this->foregroundColor(),
					gl::Transparent
					);
		}
	}
	return this->textImage_;
}

Handler<gl::TextDrawable> TextArea::descImage()
{
	if(!this->descImage_){
		if( Handler<World> w = this->world().lock() ){
			this->descImage_ = w->drawableManager()->queryText(
					this->description(),
					this->textSize_,
					Handler<gl::Font>(),
					gl::TextDrawable::Style::Bold,
					gl::TextDrawable::Decoration::None,
					this->foregroundColor().lighter(0.5),
					gl::Transparent
					);
		}
	}
	return this->descImage_;
}

geom::Box TextArea::measureImpl(geom::Box const& constraint)
{
	return this->descImage()->size() + TextMargin.totalSpace();
}

void TextArea::layoutImpl(geom::Box const& size)
{
	// XXX: What to do?
}

void TextArea::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void TextArea::text(std::string const& text)
{
	if(this->text_ != text){
		this->text_ = text;
		this->notifyViewRefreshed();
	}
}

void TextArea::description(std::string const& text)
{
	if(this->description_ != text){
		this->description_ = text;
		if(this->text().empty()) {
			this->notifyViewRefreshed();
		}
	}
}

void TextArea::textSize(float const& size)
{
	if( this->textSize_ != size ) {
		this->textSize_ = size;
		this->notifyViewRefreshed();
	}
}

bool TextArea::notifyViewRefreshedImpl()
{
	this->textImage_.reset();
	return true;
}

bool TextArea::onDownRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	if(this->editing_){
		this->startSelection(ptInScreen.x()-this->screenArea().x());
	}else{
		this->startEditing(ptInScreen.x()-this->screenArea().x());
	}
	return true;
}

bool TextArea::onMoveRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	this->moveSelection(ptInScreen.x()-this->screenArea().x());
	return true;
}

void TextArea::startEditing(float const width)
{
	this->editListBefore_.clear();
	this->editListAfter_.clear();
	this->editListEditing_.clear();
	this->editStart_=0;
	this->editLength_=0;
	this->cursorCounter_=0;
	this->editListBeforeWidth_ = 0;
	this->editListAfterWidth_ = 0;
	float left=width;
	if(Handler<World> w = this->world().lock()) {
		Handler<gl::DrawableManager> mgr(w->drawableManager());
		std::vector<std::string> lst(::tarte::breakChar(this->text_));
		auto it = lst.begin();
		for(;it != lst.end(); ++it) {
			Handler<gl::TextDrawable> t(this->createEditingText(mgr, (std::string const&)*it));
			if(left > t->width()) {
				this->editListBefore_.push_back(t);
				this->editListBeforeWidth_ += t->width();
			}else{
				this->editListAfter_.push_back(t);
				this->editListAfterWidth_ += t->width();
			}
			left -= t->width();
		}
	}
	this->editing_ = true;
}

void TextArea::stopEditing()
{
	std::stringstream ss;
	this->editListEditing_.clear();
	for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
		ss << d->str();
	}
	for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
		ss << d->str();
	}
	this->text(ss.str());
	this->editListBefore_.clear();
	this->editListAfter_.clear();
	this->editListEditing_.clear();
	this->editing_ = false;
}

void TextArea::startSelection(float width)
{
	while(this->editListBeforeWidth_ < width) {
		if(!moveCursorRight(false)){
			break;
		}
	}
	while(width < this->editListBeforeWidth_) {
		if(!moveCursorLeft(false)){
			break;
		}
	}
}

void TextArea::moveSelection(float width)
{
	while(this->editListBeforeWidth_ < width) {
		if(!moveCursorRight(true)){
			break;
		}
	}
	while(width < this->editListBeforeWidth_) {
		if(!moveCursorLeft(true)){
			break;
		}
	}
}

bool TextArea::moveCursorLeft(bool select)
{
	if(this->editListBefore_.empty()){
		return false;
	}
	Handler<gl::TextDrawable> d(this->editListBefore_.back());
	if(select){
		++this->editLength_;
	}else{
		this->editLength_ = 0;
	}
	this->editListBefore_.pop_back();
	this->editListAfter_.push_front(d);
	this->editListBeforeWidth_ -= d->width();
	this->editListAfterWidth_ += d->width();
	return true;
}
bool TextArea::moveCursorRight(bool select)
{
	if(this->editListAfter_.empty()){
		return false;
	}
	Handler<gl::TextDrawable> d(this->editListAfter_.front());
	if(select){
		--this->editLength_;
	}else{
		this->editLength_ = 0;
	}
	this->editListAfter_.pop_front();
	this->editListBefore_.push_back(d);
	this->editListBeforeWidth_ += d->width();
	this->editListAfterWidth_ -= d->width();
	return true;
}

bool TextArea::moveCursorBegin(bool select)
{
	if(this->editListBefore_.empty()){
		return false;
	}
	if(select){
		this->editLength_ += std::distance(this->editListBefore_.begin(), this->editListBefore_.end());
	}else{
		this->editLength_ = 0;
	}
	this->editListAfter_.insert(this->editListAfter_.begin(), this->editListBefore_.begin(), this->editListBefore_.end());
	this->editListBefore_.clear();
	this->editListAfterWidth_ += this->editListBeforeWidth_;
	this->editListBeforeWidth_ = 0;
	return true;
}

bool TextArea::moveCursorEnd(bool select)
{
	if(this->editListAfter_.empty()) {
		return false;
	}
	if(select){
		this->editLength_ -= std::distance(this->editListAfter_.begin(), this->editListAfter_.end());
	}else{
		this->editLength_ = 0;
	}
	this->editListBefore_.insert(this->editListBefore_.end(), this->editListAfter_.begin(), this->editListAfter_.end());
	this->editListAfter_.clear();
	this->editListBeforeWidth_ += this->editListAfterWidth_;
	this->editListAfterWidth_ = 0;
	return true;
}

bool TextArea::deleteCursorBefore()
{
	if(this->deleteSelected()){
		return true;
	}
	if(this->editListBefore_.empty()){
		return false;
	}
	Handler<gl::TextDrawable> const& d(this->editListBefore_.back());
	this->editListBeforeWidth_ -= d->width();
	this->editListBefore_.pop_back();
	this->editLength_ = 0;
	return true;
}
bool TextArea::deleteCursorAfter()
{
	if(this->deleteSelected()){
		return true;
	}
	if(this->editListAfter_.empty()){
		return false;
	}
	Handler<gl::TextDrawable> const& d(this->editListAfter_.front());
	this->editListAfterWidth_ -= d->width();
	this->editListAfter_.pop_front();
	this->editLength_ = 0;
	return true;
}

bool TextArea::deleteSelected()
{
	if(this->editLength_ < 0) {
		std::vector<Handler<gl::TextDrawable> >::iterator first = this->editListBefore_.end() + this->editLength_;
		for(std::vector<Handler<gl::TextDrawable> >::iterator it = first; it != this->editListBefore_.end();++it){
			Handler<gl::TextDrawable> const& d(*it);
			this->editListBeforeWidth_ -= d->width();
		}
		this->editListBefore_.erase(first, this->editListBefore_.end());
	}else if(this->editLength_ > 0){
		int cnt = this->editLength_;
		while(cnt > 0 && !this->editListAfter_.empty()) {
			Handler<gl::TextDrawable> const& d(this->editListAfter_.front());
			this->editListAfterWidth_ -= d->width();
			this->editListAfter_.pop_front();
			cnt--;
		}
	}else{
		return false;
	}
	this->editLength_ = 0;
	return true;
}

void TextArea::startInput()
{
	if( auto world = this->world().lock() ) {
		world->startIME(this->screenArea());
	}
}

void TextArea::stopInput()
{
	if( auto world = this->world().lock() ) {
		world->stopIME();
	}
}

Handler<gl::TextDrawable> TextArea::createEditingText(Handler<gl::DrawableManager> const& mgr, std::string const& str)
{
	return mgr->queryText(
			str,
			this->textSize_,
			Handler<gl::Font>(),
			gl::TextDrawable::Style::Bold,
			gl::TextDrawable::Decoration::None,
			this->foregroundColor(),
			gl::Transparent
	);
}

void TextArea::onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen)
{
	Element::onFocusGained(timeMs, lastPtInScreen);
	this->startInput();
}

void TextArea::onFocusLost(float const& timeMs)
{
	Element::onFocusLost(timeMs);
	this->stopInput();
	this->stopEditing();
}

void TextArea::onTextInput(float const& timeMs, std::string const& text)
{
	if(Handler<World> w = this->world().lock()) {
		Handler<gl::DrawableManager> mgr(w->drawableManager());
		for(std::string const& str : ::tarte::breakChar(text)){
			Handler<gl::TextDrawable> d(this->createEditingText(mgr, str));
			this->editListBefore_.push_back(d);
			this->editListBeforeWidth_ += d->width();
		}
	}
}
void TextArea::onTextEdit(float const& timeMs, std::string const& text, int const start, int const length)
{
	std::vector<std::string> lst(::tarte::breakChar(text));
	auto itstr = lst.begin();
	auto itspr = this->editListEditing_.begin();
	std::size_t cnt=0;
	// 一致する部分を探す。一致してる部分は描画しない。
	for(; itstr != lst.end() && itspr != this->editListEditing_.end(); ++itstr, ++itspr){
		std::string const& ch = *itstr;
		Handler<gl::TextDrawable>const& spr=*itspr;
		if(spr->str() != ch) {
			break;
		}
		++cnt;
	}
	// 一致してない部分以降を削除
	this->editListEditing_.erase(itspr, this->editListEditing_.end());
	// 一致してない部分を追加
	if(Handler<World> w = this->world().lock()) {
		Handler<gl::DrawableManager> mgr(w->drawableManager());
		for(;itstr != lst.end(); ++itstr){
			std::string const& str = *itstr;
			this->editListEditing_.push_back(this->createEditingText(mgr, str));
		}
	}
}

bool TextArea::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	switch (sym.scancode) {
	case SDL_SCANCODE_BACKSPACE:
		if(!this->deleteCursorBefore()){
			this->cursorCounter_ = 0;
		}
		break;
	case SDL_SCANCODE_DELETE:
		if(!this->deleteCursorAfter()){
			this->cursorCounter_ = 0;
		}
		break;
	case SDL_SCANCODE_LEFT:
		if(!this->moveCursorLeft(sym.mod & KMOD_SHIFT)){
			this->cursorCounter_ = 0;
		}
		break;
	case SDL_SCANCODE_RIGHT:
		if(!this->moveCursorRight(sym.mod & KMOD_SHIFT)){
			this->cursorCounter_ = 0;
		}
		break;
	case SDL_SCANCODE_UP:
		if(!this->moveCursorBegin(sym.mod & KMOD_SHIFT)){
			this->cursorCounter_ = 0;
		}
		break;
	case SDL_SCANCODE_DOWN:
		if(!this->moveCursorEnd(sym.mod & KMOD_SHIFT)){
			this->cursorCounter_ = 0;
		}
		break;
	default:
		break;
	}
	return true;
}
bool TextArea::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	return true;
}

}}
