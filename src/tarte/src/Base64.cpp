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

#include <tarte/Base64.h>
#include <tarte/Exception.h>
#include <sstream>

#if CHISA_WINDOWS
#define _LONG_LONG_INT_FORMAT_ "I64"
#elseBase64
#define _LONG_LONG_INT_FORMAT_ "ll"
#endif

namespace tarte {
namespace base64 {

static const char BASE64CHR[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char BASE64IDX[256]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
inline static unsigned char encodeHelperGet(char const* data, const std::size_t size, const unsigned int idx){
	return idx < size ? data[idx] : 0;
}

std::string encode( const char* data, const std::size_t length)
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

std::string encode( std::string const& data )
{
	return encode( data.c_str(), data.size() );
}

std::string encode( std::vector<char> const& data )
{
	return encode( data.data(), data.size() );
}

std::vector<char> decode(std::string const& str)
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

std::string decodeAsString(std::string const& str){
	const std::vector<char> dat(decode(str));
	return std::string(dat.data(), dat.size());
}

}}
