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

#include "ContentMeasurerUtil.h"
#include <unicode/unistr.h>
#include <unicode/regex.h>

namespace chisa {
namespace tk {
namespace widget {

std::string shrinkSpace(const std::string& str_)
{
	UErrorCode st = U_ZERO_ERROR;
	UnicodeString str = UnicodeString::fromUTF8(str_);
	RegexMatcher matcher(reinterpret_cast<const UChar*>(u"[\\t\\n\\f\\r ]+"), str, 0, st);
	UnicodeString out(matcher.replaceAll(reinterpret_cast<const UChar*>(u" "), st));

	std::string ret;
	out.toUTF8String(ret);
	return ret;
}

}}}

