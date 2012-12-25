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
}
Handler<gl::TextDrawable> TextArea::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock()){
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
		if( Handler<World> w = this->world().lock()){
			this->descImage_ = w->drawableManager()->queryText(
					this->description(),
					this->textSize_,
					Handler<gl::Font>(),
					gl::TextDrawable::Style::Bold,
					gl::TextDrawable::Decoration::None,
					this->foregroundColor().lighter(),
					gl::Transparent
					);
		}
	}
	return this->descImage_;
}

geom::Box TextArea::measureImpl(geom::Box const& constraint)
{
	return this->descImage()->size();
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

bool TextArea::onMoveRaw(const float timeMs, geom::Point const& ptInScreen)
{
	return true;
}

bool TextArea::onSingleTapUp(const float timeMs, geom::Point const& ptInScreen)
{
	if(this->screenArea().contain(ptInScreen)){
		this->onClick();
	}
	return true;
}

}}
