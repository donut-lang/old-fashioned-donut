/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <sstream>
#include <cinamo/Base64.h>
#include <cinamo/Exception.h>

namespace cinamo {
namespace base64 {

/**
 * BASE64での6ビットぶんの対応
 */
static const char BASE64CHR[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/**
 * デコード用
 */
static const char BASE64IDX[256]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/**
 * エンコードするためのヘルパ関数
 */
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

}}
