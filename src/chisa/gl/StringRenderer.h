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
	static constexpr float DefaultSize=16.0f;
public:
	class Command {
		DEFINE_MEMBER(private, private, bool, enabled);
		DEFINE_MEMBER(private, private, bool, vertical);
		Style style_;
		float size_;
		DEFINE_MEMBER(public, private, std::string, str);
		DEFINE_MEMBER(private, private, geom::Area, area);
	private:
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
	public:
		Command():enabled_(false),vertical_(false), style_(Style::Regular), size_(DefaultSize),str_(),area_(0,0,0,0){};
		Command(const Style style, const float size, const std::string& str, const geom::Area& area):enabled_(true),vertical_(false),style_(style), size_(size),str_(str),area_(area){};
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
		gl::Handler<gl::RawSprite> renderString(gl::Canvas& cv) const;
	};
private:
	cairo_surface_t* nullSurface_;
	cairo_t* cairo_;
	Style style_;
	float size_;
	void style(Style style);
	Style style();
	void size(float size);
	float size();
public:
	StringRenderer::Command measure(const std::string& strUtf8);
	StringRenderer::Command calcMaximumStringLength(const std::string& str, const float limit, std::size_t beginInUtf8=0, std::size_t endInUtf8=0);
public:
	StringRenderer();
	virtual ~StringRenderer();
};

}}
