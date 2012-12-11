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

#include "Util.h"
#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <unicode/brkiter.h>

namespace chisa {
namespace doc {

std::string shrinkSpace(std::string const& str_)
{
	UErrorCode st = U_ZERO_ERROR;
	UnicodeString str = UnicodeString::fromUTF8(str_);
	RegexMatcher matcher(reinterpret_cast<const UChar*>(u"[\\t\\n\\f\\r ]+"), str, 0, st);
	UnicodeString out(matcher.replaceAll(reinterpret_cast<const UChar*>(u" "), st));

	std::string ret;
	out.toUTF8String(ret);
	return ret;
}

std::vector<std::string> breakLine(std::string const& str_)
{
	UErrorCode st = U_ZERO_ERROR;
	BreakIterator* bi = BreakIterator::createLineInstance(Locale::getUS(), st);
	std::vector<std::string> list;
	UnicodeString str(UnicodeString::fromUTF8(str_));
	bi->setText(str);
	int32_t last = 0;
	int32_t p = bi->first();
	while (p != BreakIterator::DONE) {
		if(p == last){
			p = bi->next();
			continue;
		}
		std::string s;
		str.tempSubStringBetween(last, p).toUTF8String(s);
		list.push_back(s);
		last = p;
		p = bi->next();
	}
	delete bi;
	return list;
}

}}
