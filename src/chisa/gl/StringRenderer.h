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
#include <tuple>
#include "RawSprite.h"
#include "../geom/Area.h"
#include "../Handler.h"
#include "Font.h"
#include "Color.h"
#include "../util/ClassUtil.h"

namespace chisa {
namespace gl {

class StringRenderer {
public:
	enum Style {
		Regular = 0,
		Bold = 1,
		Italic = 2,
		ItalicBold = 3
	};
	static const float DefaultFontSize;
public:
	class Command {
		DEFINE_MEMBER(private, private, bool, enabled);
		DEFINE_MEMBER(private, private, bool, vertical);
		Handler<gl::Font> font_;
		Style style_;
		float size_;
		DEFINE_MEMBER(public, private, std::string, str);
		DEFINE_MEMBER(private, private, geom::Area, area);
	private:
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
	public:
		Command():enabled_(false),vertical_(false), font_(), style_(Style::Regular), size_(DefaultFontSize), str_(),area_(0,0,0,0){};
		Command(Handler<gl::Font> font, const Style style, const float size, const std::string& str, const geom::Area& area):enabled_(true),vertical_(false), font_(font),style_(style), size_(size),str_(str),area_(area){};
		Command(const Command& other) = default;
		Command(Command&& other) = default;
		Command& operator=(const Command& other) = default;
		Command& operator=(Command&& other) = default;
		Command flip() noexcept{
			Command cmd(*this);
			cmd.vertical(true);
			cmd.area(cmd.area_.flip());
			return cmd;
		}
		float width() const noexcept { return this->area().width(); };
		float height() const noexcept { return this->area().height(); };
		geom::Box size() const noexcept { return this->area().box(); };
		~Command () noexcept = default;
		explicit operator bool () const noexcept { return this->enabled(); };
	public:
		Handler<gl::RawSprite> renderString(gl::Canvas& cv) const;
	};
private:
	Handler<gl::FontManager> fontManager_;
	Handler<gl::Font> font_;
	cairo_font_face_t* face_;
	cairo_surface_t* nullSurface_;
	cairo_font_options_t* nullOption_;
	cairo_t* cairo_;
	std::vector<Style> styleStack_;
	std::vector<float> sizeStack_;
	std::vector<std::string> fontStack_;
	std::vector<gl::Color> colorStack_;
public:
	void pushStyle(Style style);
	Style nowStyle() const;
	void popStyle();

	void pushSize(float size);
	float nowSize() const;
	void popSize();

	void pushColor(const Color& c);
	Color nowColor() const;
	void popColor();

	void pushFont( const std::string& name );
	std::string nowFont() const;
	void popFont();
public:
	StringRenderer::Command measure(const std::string& strUtf8);
	StringRenderer::Command calcMaximumStringLength(const std::string& str, const float limit, std::size_t beginInUtf8=0, std::size_t endInUtf8=0);
public:
	StringRenderer(Handler<gl::FontManager> fontManager);
	virtual ~StringRenderer() noexcept;
private:
	static void setupCairo(cairo_t* cairo, cairo_font_face_t* face, cairo_font_options_t* opt, float size, Style style);
};

}}
