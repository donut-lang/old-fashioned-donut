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
#include <cairo/cairo.h>
#include <cstdint>
#include <cstddef>
#include "../../logging/Logger.h"
#include "../../gl/DrawableManager.h"
#include "../../geom/Vector.h"
#include "../../gl/Color.h"
#include "../render/RenderTree.h"

namespace chisa {
namespace doc {

class TextContext {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	Handler<RenderTree> renderTree_;
	Handler<gl::Font> font_;
	cairo_font_face_t* face_;
	cairo_surface_t* nullSurface_;
	cairo_font_options_t* nullOption_;
	cairo_t* cairo_;
	std::vector<gl::TextDrawable::Style> styleStack_;
	std::vector<gl::TextDrawable::Decoration> decoStack_;
	std::vector<float> sizeStack_;
	std::vector<std::string> fontStack_;
	std::vector<gl::Color> colorStack_;
	std::vector<gl::Color> backColorStack_;
public:
	void pushStyle(gl::TextDrawable::Style style);
	gl::TextDrawable::Style nowStyle() const;
	void popStyle();

	void pushDeco(gl::TextDrawable::Decoration deco);
	gl::TextDrawable::Decoration nowDeco() const;
	void popDeco();

	void pushSize(float size);
	float nowSize() const;
	void popSize();

	void pushColor(const gl::Color& c);
	gl::Color nowColor() const;
	void popColor();

	void pushBackColor(const gl::Color& c);
	gl::Color nowBackColor() const;
	void popBackColor();

	void pushFont( const std::string& name );
	std::string nowFont() const;
	void popFont();
public:
	geom::Box measure(const std::string& strUtf8);
	Handler<TextDrawableObject> create(const std::string& strUtf8, HandlerW<Node> parentNode, const float relDepth );
public:
	TextContext(logging::Logger& log, Handler<RenderTree> renderTree);
	virtual ~TextContext() noexcept;
};

}}
