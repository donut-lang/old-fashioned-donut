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

static constexpr float Space = 1.5f;

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TextArea, Element)
,textSize_(32.0f)
,description_()
,text_()
,textImage_()
,descImage_()
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

void TextArea::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	canvas.fillRect(gl::White, screenArea);
	if( this->onFocused() ) {
		canvas.drawRect(2.0f, gl::DarkYellow, screenArea);
		geom::Distance pos(screenArea.point());
		if(this->editListEditing_.empty()){
			for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
				Handler<gl::Sprite> spr(d->sprite());
				canvas.drawSprite(spr, pos+geom::Distance(0, screenArea.height() - Space - spr->height()));
				pos.x(pos.x() + d->width());
			}
			for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
				Handler<gl::Sprite> spr(d->sprite());
				canvas.drawSprite(spr, pos+geom::Distance(0, screenArea.height() - Space - spr->height()));
				pos.x(pos.x() + d->width());
			}
		}else{
			for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
				Handler<gl::Sprite> spr(d->sprite());
				canvas.drawSprite(spr, pos+geom::Distance(0, screenArea.height() - Space - spr->height()));
				pos.x(pos.x() + d->width());
			}
			int cnt = 0;
			geom::Area editArea;
			for(Handler<gl::TextDrawable> const& d : this->editListEditing_) {
				if(this->editStart_ == cnt) {
					editArea.point() = pos;
				}
				Handler<gl::Sprite> spr(d->sprite());
				canvas.drawSprite(spr, pos+geom::Distance(0, screenArea.height() - Space - spr->height()));
				pos.x(pos.x() + d->width());
				if(this->editStart_+this->editLength_ == cnt) {
					editArea.box() = geom::Box(pos.x()-editArea.x(), screenArea.height());
					canvas.fillRect(gl::Black, editArea);
				}
				++cnt;
			}
			for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
				Handler<gl::Sprite> spr(d->sprite());
				canvas.drawSprite(spr, pos+geom::Distance(0, screenArea.height() - Space - spr->height()));
				pos.x(pos.x() + d->width());
			}
		}
	}else{
		canvas.drawRect(2.0f, gl::Black, screenArea);
		if(this->text_.empty()) {
			Handler<gl::Sprite> spr(this->descImage()->sprite());
			canvas.drawSprite(spr, screenArea.point()+(screenArea.box() - spr->size())/2.0f);
		}else{
			Handler<gl::Sprite> spr(this->textImage()->sprite());
			canvas.drawSprite(spr, screenArea.point()+geom::Distance(Space, Space));
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
	return this->descImage()->size() + geom::Box(Space*2, Space*2);
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

void TextArea::onClick()
{
}

bool TextArea::notifyViewRefreshedImpl()
{
	this->textImage_.reset();
	return true;
}

bool TextArea::onMoveRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	return true;
}

bool TextArea::onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen)
{
	if(this->screenArea().contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

void TextArea::startEditing()
{
	this->editListBefore_.clear();
	this->editListAfter_.clear();
	this->appendEditingText(this->editListBefore_, ::tarte::breakChar(this->text_));
}

void TextArea::appendEditingText(std::vector<Handler<gl::TextDrawable> >& append, std::vector<std::string> const& lst)
{
	if( Handler<World> w = this->world().lock() ){
		Handler<gl::DrawableManager> mgr ( w->drawableManager() );
		for(std::string const& c : lst){
			append.push_back(mgr->queryText(
				c,
				this->textSize_,
				Handler<gl::Font>(),
				gl::TextDrawable::Style::Bold,
				gl::TextDrawable::Decoration::None,
				this->foregroundColor(),
				gl::Transparent
			));
		}
	}
}


void TextArea::stopEditing()
{
	std::stringstream ss;
	for(Handler<gl::TextDrawable> const& d : this->editListBefore_) {
		ss << d->str();
	}
	for(Handler<gl::TextDrawable> const& d : this->editListAfter_) {
		ss << d->str();
	}
	this->text(ss.str());
	this->editListBefore_.clear();
	this->editListAfter_.clear();
}

void TextArea::onFocusGained(float const& timeMs)
{
	Element::onFocusGained(timeMs);
	if( auto world = this->world().lock() ) {
		world->startIME(this->screenArea());
		this->startEditing();
	}
}

void TextArea::onFocusLost(float const& timeMs)
{
	Element::onFocusLost(timeMs);
	if( auto world = this->world().lock() ) {
		world->stopIME();
		this->stopEditing();
	}
}

void TextArea::onTextInput(float const& timeMs, std::string const& text)
{
	this->appendEditingText(this->editListBefore_, ::tarte::breakChar(text));
}
void TextArea::onTextEdit(float const& timeMs, std::string const& text, int const start, int const length)
{
	std::size_t cnt=0;
	std::vector<std::string> lst(::tarte::breakChar(text));
	auto it = lst.begin();
	for(; it != lst.end(); ++it){
		std::string const& ch = *it;
		if(this->editListEditing_.size() <= cnt || ((Handler<gl::TextDrawable>const&)this->editListEditing_[cnt])->str() != ch) {
			break;
		}
		++cnt;
	}
	this->editListEditing_.erase(this->editListEditing_.begin()+cnt, this->editListEditing_.end());
	lst.erase(lst.begin(), it);
	this->appendEditingText(this->editListEditing_, lst);
}

bool TextArea::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	if(sym.scancode == SDL_SCANCODE_BACKSPACE) {
		this->editListBefore_.pop_back();
	}
	return true;
}
bool TextArea::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	return true;
}

}}
