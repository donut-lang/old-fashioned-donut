/**
 * Tarte
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

#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <algorithm>
#include <sstream>

#include <tarte/Exception.h>
#include <tarte/Platform.h>
#include <tarte/String.h>

#if CHISA_WINDOWS
#define _LONG_LONG_INT_FORMAT_ "I64"
#else
#define _LONG_LONG_INT_FORMAT_ "ll"
#endif

namespace tarte {

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
		TARTE_EXCEPTION(Exception, "Format string too long!!");
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
		TARTE_EXCEPTION(Exception, "[BUG] Unknwon radix: %d", radix);\
	}\
}

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

static const char BASE64CHR[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char BASE64IDX[256]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
inline static unsigned char encodeHelperGet(char const* data, const std::size_t size, const unsigned int idx){
	return idx < size ? data[idx] : 0;
}

std::string encodeBase64( const char* data, const std::size_t length)
{
	std::stringstream ss;
	std::size_t nowbyte = 0;
	std::size_t nowbit = 0;

	std::size_t cnt = 0;
	while(nowbyte < length){
		if(nowbit <= 2){
			const int a = encodeHelperGet(data, length, nowbyte);
			ss << BASE64CHR[(a >> (2-nowbit)) & 63];
		}else{
			const int a = encodeHelperGet(data, length, nowbyte);
			const int b = encodeHelperGet(data, length, nowbyte+1);
			const int da = (a & (255 >> nowbit)) << (6-(8-nowbit));
			const int db = b>> (10-nowbit);
			const int c = db | da;
			ss << BASE64CHR[ c ];
		}
		nowbit += 6;
		nowbyte += (nowbit >> 3);
		nowbit &= 7;
		++cnt;
	}
	while((cnt & 3) != 0){
		ss << "=";
		++cnt;
	}
	return ss.str();
}

std::string encodeBase64( std::string const& data )
{
	return encodeBase64( data.c_str(), data.size() );
}

std::string encodeBase64( std::vector<char> const& data )
{
	return encodeBase64( data.data(), data.size() );
}

std::vector<char> decodeBase64(std::string const& str)
{
	std::vector<char> dat;

	char now = 0;
	int nowbit = 0;

	for(unsigned char c : str){
		if(c == '='){
			nowbit = 0;
			break;
		} else {
			unsigned char const b = BASE64IDX[c];
			if(nowbit <= 2){
				now |= b << (2-nowbit);
				nowbit += 6;
			}else{
				now |= (b >> (nowbit-2));
				nowbit += 6;
				if(nowbit >= 8){
					dat.push_back(now);
					now = 0;
					nowbit -= 8;
				}
				now |= b << (8-nowbit);
			}
		}
	}
	if(nowbit > 0){
		dat.push_back(now);
	}

	return dat;
}

std::string decodeBase64AsString(std::string const& str){
	const std::vector<char> dat(decodeBase64(str));
	return std::string(dat.data(), dat.size());
}

#define PARSE_STRTO(TYPE, FUNC) \
template <>\
TYPE parseAs<TYPE>(std::string const& str, int radix, bool* succeed) {\
	char* end;\
	TYPE const result = std::FUNC(str.c_str(), &end, radix);\
	if((*end) != '\0'){\
		if(succeed != nullptr){\
			*succeed = false;\
		}else{\
			TARTE_EXCEPTION(Exception, "Invalid number: %s", str.c_str());\
		}\
	} else {\
		if(succeed) *succeed = true;\
	}\
	return result;\
}\
template <>\
TYPE parseAs<TYPE>(std::string const& str, bool* succeed) {\
	return parseAs<TYPE>(str, 0, succeed);\
}

PARSE_STRTO(int, strtol);
PARSE_STRTO(unsigned int, strtoul);

PARSE_STRTO(long int, strtol);
PARSE_STRTO(long unsigned int, strtoul);

PARSE_STRTO(long long int, strtoll);
PARSE_STRTO(unsigned long long int, strtoull);


#define PARSE_STRTO_F(TYPE, FUNC) \
	template <> TYPE parseAs<TYPE>(std::string const& str, bool* succeed)\
	{\
		char* end;\
		TYPE const result = FUNC(str.c_str(), &end);\
		if((*end) != '\0'){\
			if(succeed != nullptr){\
				*succeed = false;\
			}else{\
				TARTE_EXCEPTION(Exception, "Invalid number: %s", str.c_str());\
			}\
		} else {\
			if(succeed) *succeed = true;\
		}\
		return result;\
	}


PARSE_STRTO_F(float, strtof);
PARSE_STRTO_F(double, strtod);
PARSE_STRTO_F(long double, strtold);

#undef PARSE_STRTO_F
#undef PARSE_STRTO

template <>
bool parseAs<bool>(std::string const& str, bool* succeed)
{
	std::string copy = toLower(str);
	if( copy == "true" || copy == "yes") {
		if(succeed) *succeed = true;
		return true;
	} else if(copy == "false" || copy=="no") {
		if(succeed) *succeed = true;
		return false;
	} else if(succeed){
		*succeed = false;
	} else {
		TARTE_EXCEPTION(Exception, "Invalid boolean: %s", str.c_str());
	}
	return false;
}

std::string decodePercent(std::string const& str)
{
	char* from(new char[str.length()+1]);
	char* to(new char[str.length()+1]);
	std::memcpy(from, str.c_str(), str.length());
	from[str.length()] = '\0';
	size_t i=0,j=0;
	for(;i<str.length();++i, ++j){
		if(from[i] == '%'){
			char code[3];
			code[0] = from[i+1];
			code[1] = from[i+2];
			code[2] = '\0';
			i+=2;
			char* failed = 0;
			to[j] = static_cast<char>(std::strtol(code, &failed, 16) & 0xff);
			if(failed != &code[2]){
				TARTE_EXCEPTION(Exception, "Failed to decode percent: %s", str.c_str());
			}
		}else if(from[i]=='+'){
			to[j] = ' ';
		}else{
			to[j] = from[i];
		}
	}
	to[j]='\0';

	std::string res(to);

	delete [] from;
	delete [] to;

	return res;
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

void split(std::string const& str, std::string const& sep, std::vector<std::string>& list)
{
	if(sep.size() <= 0){
		return;
	}
	size_t i = 0;
	size_t found = 0;
	while((found = str.find(sep, i)) != std::string::npos){
		if(found != i){
			list.push_back(str.substr(i, found-i));
		}
		i = found+sep.length();
	}
	if(i != str.size()){
		list.push_back(str.substr(i));
	}
}

static size_t findFirstOf(std::string const& str, const std::string* sep, size_t n, size_t from, size_t* offset, size_t* seplen)
{
	*offset = std::string::npos;
	for(size_t i = 0;i<n;++i){
		size_t found = str.find(sep[i], from);
		if(sep[i].size() > 0 && found != std::string::npos && (found < *offset || *offset == std::string::npos)){
			*offset = found;
			*seplen = sep[i].size();
		}
	}
	return *offset;
}

void split(std::string const& str, const std::string* sep, size_t n, std::vector<std::string>& list)
{
	size_t i = 0;
	size_t found = std::string::npos;
	size_t seplen = std::string::npos;
	while((findFirstOf(str, sep, n, i, &found, &seplen)) != std::string::npos){
		if(found != i){
			list.push_back(str.substr(i, found-i));
		}
		i = found+seplen;
	}
	if(i != str.size()){
		list.push_back(str.substr(i));
	}
}
void splitLine(std::string const& str, std::vector<std::string>& list)
{
	static const std::string lineSep[] = {
			"\n",
			"\r"
	};
	split(str, lineSep, list);
}
void splitSpace(std::string const& str, std::vector<std::string>& list)
{
	static const std::string spaces[] = {
			" ",
			"　"
	};
	split(str, spaces, list);
}

}
