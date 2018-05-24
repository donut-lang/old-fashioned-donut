/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cstdlib>
#include <cinamo/Exception.h>
#include <cinamo/String.h>
#include <cinamo/Platform.h>

namespace cinamo {

#define PARSE_STRTO(TYPE, FUNC) \
template <>\
bool tryParseAs<TYPE>(std::string const& str, int radix, TYPE& result) {\
	char* end;\
	TYPE r = FUNC(str.c_str(), &end, radix);\
	if(str.length() > 0 && (*end) == '\0'){\
		result = r;\
		return true;\
	}else{\
		return false;\
	}\
}\
template <>\
bool tryParseAs<TYPE>(std::string const& str, TYPE& result) {\
	return tryParseAs<TYPE>(str, 0, result);\
}

PARSE_STRTO(char, strtol);
PARSE_STRTO(signed char, strtol);
PARSE_STRTO(unsigned char, strtoul);

PARSE_STRTO(short, strtol);
PARSE_STRTO(unsigned short, strtoul);

PARSE_STRTO(int, strtol);
PARSE_STRTO(unsigned int, strtoul);

PARSE_STRTO(long int, strtol);
PARSE_STRTO(long unsigned int, strtoul);

#if defined(HAVE_STRTOLL)
PARSE_STRTO(long long int, strtoll);
#endif
#if defined(HAVE_STRTOULL)
PARSE_STRTO(unsigned long long int, strtoull);
#endif

#define PARSE_STRTO_F(TYPE, FUNC) \
	template <> bool tryParseAs<TYPE>(std::string const& str, TYPE& result)\
	{\
		char* end;\
		TYPE r = FUNC(str.c_str(), &end);\
		if(str.length() > 0 && (*end) == '\0'){\
			result=r;\
			return true;\
		}else{\
			return false;\
		}\
	}


PARSE_STRTO_F(float, strtof);
PARSE_STRTO_F(double, strtod);
#ifdef HAVE_LONG_DOUBLE
PARSE_STRTO_F(long double, strtold);
#endif

#undef PARSE_STRTO_F
#undef PARSE_STRTO

template <>
bool tryParseAs<bool>(std::string const& str, bool& result)
{
	std::string copy = toLower(str);
	if( copy == "true" || copy == "yes") {
		result = true;
		return true;
	} else if(copy == "false" || copy=="no") {
		result = false;
		return true;
	}else{
		return false;
	}
}

}
