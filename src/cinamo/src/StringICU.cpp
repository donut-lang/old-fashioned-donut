/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Platform.h>

#ifdef HAVE_ICU

#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <unicode/brkiter.h>

#include <cinamo/String.h>

namespace cinamo {

std::vector<std::string> breakChar(std::string const& str_)
{
	UErrorCode st = U_ZERO_ERROR;
	BreakIterator* bi = BreakIterator::createCharacterInstance(Locale::getJapanese(), st);
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

std::string trim(std::string const& str_)
{
	std::string tmp(str_);
	return trim(tmp);
}
std::string& trim(std::string& str_)
{
	UnicodeString str(UnicodeString::fromUTF8(str_));
	str.trim();
	str_.clear();
	str.toUTF8String(str_);
	return str_;
}

}
#endif
