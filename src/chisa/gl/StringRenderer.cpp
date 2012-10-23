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

#include "StringRenderer.h"
#include "../geom/Vector.h"
#include <cfloat>
#include <cstring>
#include <unicode/unistr.h>
#include <unicode/bytestream.h>

namespace chisa {
namespace gl {

StringRenderer::StringRenderer()
:nullSurface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,cairo_(cairo_create(nullSurface_))
{
}

StringRenderer::~StringRenderer()
{
	cairo_destroy(this->cairo_);
	cairo_surface_destroy(this->nullSurface_);
}

geom::Area StringRenderer::measure(const char* utf8, std::size_t begin, std::size_t end)
{
	cairo_text_extents_t ext;
	if(end != SIZE_MAX){
		const int len = end-begin;
		char* buf = new char[len+1];
		std::memcpy(buf, &utf8[begin], len);
		buf[len] = 0;
		cairo_text_extents(this->cairo_, buf, &ext);
		delete [] buf;
	}else{
		cairo_text_extents(this->cairo_, &utf8[begin], &ext);
	}
	auto offset = geom::Vector(ext.x_bearing, -ext.y_bearing);
	auto size = geom::Box(ext.x_advance, ext.height+ext.y_advance);
	return geom::Area(offset, size);
}

std::tuple<geom::Area, size_t, size_t> StringRenderer::calcMaximumStringLength(const std::string& ostr, const float limit, size_t beginInUtf8, size_t endInUtf8)
{
	UnicodeString str(UnicodeString::fromUTF8(ostr));
	const size_t len = ostr.length();
	char* const buf = new char[len+1];
	CheckedArrayByteSink sink(buf, len+1);

	std::size_t const beg = beginInUtf8;
	std::size_t const end = std::min(static_cast<size_t>(str.length()), endInUtf8);
	geom::Area area;

	sink.Reset();
	str.tempSubStringBetween(beg, end).toUTF8(sink);
	area = this->measure(buf);
	if(area.width() <= limit){
		return std::tuple<geom::Area, size_t, size_t>(area, end, end);
	}

	std::size_t min = beg;
	std::size_t max = end;

	while(min+1 < max){
		sink.Reset();
		std::size_t center = (min+max)/2;
		str.tempSubStringBetween(beg, center).toUTF8(sink);
		area = this->measure(buf);
		if(area.width() <= limit){
			min = center;
		}else{
			max = center;
		}
	}
	delete [] buf;
	return std::tuple<geom::Area, size_t, size_t>(area, min, end);
}

}}
