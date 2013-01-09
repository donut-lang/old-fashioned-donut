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

#include <tarte/Exception.h>
#include "TextContext.h"
#include <unicode/unistr.h>
#include <unicode/bytestream.h>
#include "../../gl/Drawable.h"
#include "../render/RenderObject.h"

namespace chisa {
namespace doc {

TextContext::TextContext(Logger& log, Handler<RenderTree> renderTree)
:log_(log)
,renderTree_(renderTree)
,font_()
{
	this->pushStyle(gl::TextDrawable::Style::Regular);
	this->pushDeco(gl::TextDrawable::Decoration::None);
	this->pushSize(gl::TextDrawable::DefaultFontSize);
	this->pushFont("");
	this->pushColor(gl::Black);
	this->pushBackColor(gl::Transparent);
}

TextContext::~TextContext() noexcept
{
	this->font_.reset();
	this->renderTree_.reset();
}

void TextContext::pushStyle(gl::TextDrawable::Style style)
{
	this->styleStack_.push_back(style);
}

gl::TextDrawable::Style TextContext::nowStyle() const
{
	return this->styleStack_.back();
}

void TextContext::popStyle()
{
	this->styleStack_.pop_back();
}

void TextContext::pushSize(float size)
{
	this->sizeStack_.push_back(size);
}

float TextContext::nowSize() const
{
	return this->sizeStack_.back();
}

void TextContext::popSize()
{
	this->sizeStack_.pop_back();
}

void TextContext::pushColor(gl::Color const& c)
{
	this->colorStack_.push_back(c);
}
gl::Color TextContext::nowColor() const
{
	return this->colorStack_.back();
}
void TextContext::popColor()
{
	this->colorStack_.pop_back();
}


void TextContext::pushBackColor(gl::Color const& c)
{
	this->backColorStack_.push_back(c);
}
gl::Color TextContext::nowBackColor() const
{
	return this->backColorStack_.back();
}
void TextContext::popBackColor()
{
	this->backColorStack_.pop_back();
}

void TextContext::pushDeco(gl::TextDrawable::Decoration deco)
{
	this->decoStack_.push_back(deco);
}
gl::TextDrawable::Decoration TextContext::nowDeco() const
{
	return this->decoStack_.back();
}
void TextContext::popDeco()
{
	this->decoStack_.pop_back();
}

void TextContext::pushFont( std::string const& name )
{
	this->fontStack_.push_back(name);
	this->font_ = this->renderTree_->drawableManager()->queryFont(name);

}

std::string TextContext::nowFont() const
{
	return this->fontStack_.back();
}

void TextContext::popFont()
{
	this->fontStack_.pop_back();
	const std::string name = this->fontStack_.back();
	this->font_ = this->renderTree_->drawableManager()->queryFont(name);
}

Handler<TextDrawableObject> TextContext::create(std::string const& strUtf8, Node* parentNode, const float relDepth )
{
	return Handler<TextDrawableObject> ( new TextDrawableObject (
			this->renderTree_,
			parentNode,
			relDepth,
			strUtf8,
			this->nowSize(),
			this->font_,
			this->nowStyle(),
			this->nowDeco(),
			this->nowColor(),
			this->nowBackColor()
	) );
}

}}
