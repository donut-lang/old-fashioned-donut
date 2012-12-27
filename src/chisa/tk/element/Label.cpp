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

#include "Label.h"
#include "../../gl/Canvas.h"
#include "../../gl/DrawableManager.h"
#include "../World.h"

#include "ElementGroup.h"
namespace chisa {
namespace tk {

const std::string Label::AttrName::Text("text");
const std::string Label::AttrName::TextSize("text-size");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Label, Element)
,text_()
,textSize_(32.0f)
,vertical_(false)
,textImage_()
{
	this->margin(geom::Space(2.5f));
	this->padding(geom::Space(2.5f));
	this->edgeWidth(0);
	this->edgeColor(gl::Transparent);
	this->addAttribute(AttrName::Text, this->text_);
	this->addAttribute(AttrName::TextSize, this->textSize_);
}

Label::~Label() noexcept
{
}

std::string Label::toString() const
{
	return ::tarte::format("(Label text:\"%s\" %p)", this->text_.c_str(), this);
}

void Label::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	this->textImage()->draw(canvas, screenArea);
}
Handler<gl::TextDrawable> Label::textImage()
{
	if(!this->textImage_){
		if( Handler<World> w = this->world().lock() ){
			if(vertical_){
				this->textImage_ = w->drawableManager()->queryVerticalText(
						this->text(),
						this->textSize_,
						Handler<gl::Font>(),
						gl::TextDrawable::Style::Bold,
						gl::TextDrawable::Decoration::None,
						this->foregroundColor(),
						gl::Transparent
						);
			}else{
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
	}
	return this->textImage_;
}
geom::Box Label::measureImpl(geom::Box const& constraint)
{
	return this->textImage()->size();
}

void Label::layoutImpl(geom::Box const& size)
{
}
void Label::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
}

void Label::text(std::string const& text)
{
	if(this->text_ != text){
		this->text_ = text;
		this->notifyViewRefreshed();
	}
}

void Label::textSize(float const& size)
{
	if( this->textSize_ != size ) {
		this->textSize_ = size;
		this->notifyViewRefreshed();
	}
}
void Label::setVertical(bool isVertical)
{
	if( this->vertical_ != isVertical ) {
		this->vertical_ = isVertical;
		this->notifyViewRefreshed();
	}
}

bool Label::notifyViewRefreshedImpl()
{
	this->textImage_.reset();
	return true;
}

}}
