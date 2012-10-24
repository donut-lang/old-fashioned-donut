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
	class Command {
		DEFINE_MEMBER(public, private, bool, enabled);
		DEFINE_MEMBER(public, private, bool, vertical);
		DEFINE_MEMBER(public, private, std::string, str);
		DEFINE_MEMBER(public, private, geom::Area, area);
	private:
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
	public:
		Command():enabled_(false),vertical_(false),str_(),area_(0,0,0,0){};
		Command(const std::string& str, geom::Area& area):enabled_(true),vertical_(false),str_(str),area_(area){};
		Command(const std::string& str, geom::Area&& area):enabled_(true),vertical_(false),str_(str),area_(area){};
		Command(const std::string&& str, geom::Area& area):enabled_(true),vertical_(false),str_(str),area_(area){};
		Command(const std::string&& str, geom::Area&& area):enabled_(true),vertical_(false),str_(str),area_(area){};
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
		~Command () noexcept = default;
		explicit operator bool () const noexcept { return this->enabled(); };
	};
private:
	cairo_surface_t* nullSurface_;
	cairo_t* cairo_;
public:
	StringRenderer::Command measure(const std::string& strUtf8);
	StringRenderer::Command calcMaximumStringLength(const std::string& str, const float limit, std::size_t beginInUtf8=0, std::size_t endInUtf8=0);
	gl::Handler<gl::RawSprite> renderString(gl::Canvas& cv, const StringRenderer::Command& cmd);
public:
	StringRenderer();
	virtual ~StringRenderer();
};

}}
