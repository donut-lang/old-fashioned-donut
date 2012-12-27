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
	this->startEditing(ptInScreen.x()-this->screenArea().x());
	return true;
}

bool TextArea::onMoveRaw(float const& timeMs, geom::Point const& ptInScreen)
{
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
	float left=width;
	if(Handler<World> w = this->world().lock()) {
		Handler<gl::DrawableManager> mgr(w->drawableManager());
		std::vector<std::string> lst(::tarte::breakChar(this->text_));
		auto it = lst.begin();
		for(;it != lst.end(); ++it) {
			Handler<gl::TextDrawable> t(this->createEditingText(mgr, (std::string const&)*it));
			if(left > t->width()) {
				this->editListBefore_.push_back(t);
			}else{
				this->editListAfter_.push_back(t);
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
	std::cout << "lost" << std::endl;
	this->stopInput();
	this->stopEditing();
}

void TextArea::onTextInput(float const& timeMs, std::string const& text)
{
	if(Handler<World> w = this->world().lock()) {
		Handler<gl::DrawableManager> mgr(w->drawableManager());
		for(std::string const& str : ::tarte::breakChar(text)){
			this->editListBefore_.push_back(this->createEditingText(mgr, str));
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
		if(this->editListBefore_.empty()){
			this->cursorCounter_ = 0;
		}else{
			this->editListBefore_.pop_back();
		}
		break;
	case SDL_SCANCODE_DELETE:
		if(this->editListAfter_.empty()){
			this->cursorCounter_ = 0;
		}else{
			this->editListAfter_.pop_front();
		}
		break;
	case SDL_SCANCODE_LEFT:
		if(this->editListBefore_.empty()){
			this->cursorCounter_ = 0;
		}else{
			Handler<gl::TextDrawable> d(this->editListBefore_.back());
			this->editListBefore_.pop_back();
			this->editListAfter_.push_front(d);
		}
		break;
	case SDL_SCANCODE_RIGHT:
		if(this->editListAfter_.empty()){
			this->cursorCounter_ = 0;
		}else{
			Handler<gl::TextDrawable> d(this->editListAfter_.front());
			this->editListAfter_.pop_front();
			this->editListBefore_.push_back(d);
		}
		break;
	case SDL_SCANCODE_UP:
		if(this->editListBefore_.empty()){
			this->cursorCounter_ = 0;
		}else{
			this->editListAfter_.insert(this->editListAfter_.begin(), this->editListBefore_.begin(), this->editListBefore_.end());
			this->editListBefore_.clear();
		}
		break;
	case SDL_SCANCODE_DOWN:
		if(this->editListAfter_.empty()) {
			this->cursorCounter_ = 0;
		} else {
			this->editListBefore_.insert(this->editListBefore_.end(), this->editListAfter_.begin(), this->editListAfter_.end());
			this->editListAfter_.clear();
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
