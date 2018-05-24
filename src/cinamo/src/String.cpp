/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <algorithm>
#include <sstream>

#include <cinamo/Exception.h>
#include <cinamo/Platform.h>
#include <cinamo/String.h>

#if CINAMO_WINDOWS
#define _LONG_LONG_INT_FORMAT_ "I64"
#else
#define _LONG_LONG_INT_FORMAT_ "ll"
#endif

namespace cinamo {

std::string format(std::string const& fmt, ...)
{
	va_list lst;
	va_start(lst, fmt);
	std::string res(formatv(fmt, lst));
	va_end(lst);
	return res;
}
std::string formatv(std::string const& fmt, va_list args)
{
	char buff[8192];
	size_t len = vsnprintf(buff, 8192, fmt.c_str(), args);
	if(len < 0){
		CINAMO_EXCEPTION(Exception, "Format string too long!!");
	}
	return buff;
}

#define TOSTR_DEF(type, fmtstr8, fmtstr10, fmtstr16)\
std::string toString(type const& val, int radix) {\
	switch(radix){\
	case 0:\
		return format(fmtstr10, val);\
	case 8:\
		return format(fmtstr8, val);\
	case 10:\
		return format(fmtstr10, val);\
	case 16:\
		return format(fmtstr16, val);\
	default:\
		CINAMO_EXCEPTION(Exception, "[BUG] Unknwon radix: %d", radix);\
		return "dummy for ignore warning.";\
	}\
}

#if ((CHAR_MIN) == 0) //CHARが符号付きかどうかは環境依存です。
TOSTR_DEF(char, "0%o", "%u", "0x%x");
#else
TOSTR_DEF(char, "0%o", "%d", "0x%x");
#endif
TOSTR_DEF(unsigned char, "0%o", "%u", "0x%x");
TOSTR_DEF(signed char, "0%o", "%d", "0x%x");

TOSTR_DEF(int, "0%o", "%d", "0x%x");
TOSTR_DEF(unsigned int, "0%o", "%u", "0x%x");
// メモリモデルによってlong intの値は違う
#if INT_MAX == LONG_MAX
TOSTR_DEF(long int, "0%o", "%d", "0x%x");
TOSTR_DEF(unsigned long int, "0%o", "%u", "0x%x");
#elif LONG_MAX > INT_MAX
TOSTR_DEF(long int, "0%" _LONG_LONG_INT_FORMAT_ "o", "%" _LONG_LONG_INT_FORMAT_ "d", "0x%" _LONG_LONG_INT_FORMAT_ "x");
TOSTR_DEF(unsigned long int, "0%" _LONG_LONG_INT_FORMAT_ "o", "%" _LONG_LONG_INT_FORMAT_ "u", "0x%" _LONG_LONG_INT_FORMAT_ "x");
#else
#error "Unknwon data type."
#endif

TOSTR_DEF(long long int, "0%" _LONG_LONG_INT_FORMAT_ "o", "%" _LONG_LONG_INT_FORMAT_ "d", "0x%" _LONG_LONG_INT_FORMAT_ "x");
TOSTR_DEF(unsigned long long int, "0%" _LONG_LONG_INT_FORMAT_ "o", "%" _LONG_LONG_INT_FORMAT_ "u", "0x%" _LONG_LONG_INT_FORMAT_ "x");

#undef TOSTR_DEF

std::string toString(const float val)
{
	if(!(val == val)){
		return "nan";
	}else if(!std::isfinite(val)){
		return val < 0 ? "-infinity" : "infinity";
	}
	return format("%f", val);
}

std::string toString(const double val)
{
	if(!(val == val)){
		return "nan";
	}else if(!std::isfinite(val)){
		return val < 0 ? "-infinity" : "infinity";
	}
	return format("%f", val);
}

std::string toString(const bool val)
{
	return val ? "true" : "false";
}

std::string join(std::vector<std::string> const& lst)
{
	std::stringstream ss;
	for(std::string const& str : lst) {
		ss << str;
	}
	return ss.str();
}

std::string join(std::vector<std::string> const& lst, std::string const& sep)
{
	std::stringstream ss;
	bool isNotFirst = false;
	for(std::string const& str : lst) {
		if(isNotFirst){
			ss << sep;
		}
		ss << str;
		isNotFirst = true;
	}
	return ss.str();
}

std::size_t matchString(std::string const& a, std::string const& b)
{
	std::size_t t = 0;
	for(std::string::value_type const& c : a){
		if(c != b[t]) {
			break;
		}
		++t;
	}
	return t;
}

const static char RandChar[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9',};

std::string randomString(std::size_t len)
{
	std::stringstream ss;
	for(std::size_t i = 0;i<len;++i){
		ss << RandChar[ std::rand() % (sizeof(RandChar)/sizeof(char)) ];
	}
	return ss.str();
}

std::string toLower(std::string const& str)
{
	std::string copy(str);
	std::transform(copy.begin(), copy.end(), copy.begin(), (int (*)(int))std::tolower);
	return copy;
}

std::string toUpper(std::string const& str)
{
	std::string copy(str);
	std::transform(copy.begin(), copy.end(), copy.begin(), (int (*)(int))std::toupper);
	return copy;
}

bool startsWith(std::string const& target, std::string const& prefix)
{
	return target.compare(0, prefix.size(), prefix) == 0;
}
bool endsWith(std::string const& target, std::string const& suffix)
{
	return target.compare(target.size()-suffix.size(), suffix.size(), suffix) == 0;
}

}
