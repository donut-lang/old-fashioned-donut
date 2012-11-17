/**
 * Nekomata
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

#include <string>
#include <cstdlib>
#include <sstream>
#include <antlr3.h>
#include "../../logging/Exception.h"
#include "ParseUtil.h"

namespace chisa{
namespace donut{

const std::string createStringFromString(pANTLR3_STRING string){
	if(string->encoding != ANTLR3_ENC_UTF8){
		pANTLR3_STRING str = string->toUTF8(string);
		return std::string(reinterpret_cast<const char*>(str->chars));
	}
	return std::string(reinterpret_cast<const char*>(string->chars));
}
const std::string createStringFromToken(pANTLR3_COMMON_TOKEN tok){
	return createStringFromString(tok->getText(tok));
}

const std::string unescapeString(const std::string& str){
	std::stringstream ss;
	size_t last = 0;
	size_t idx;
	while(((idx = str.find("\\", last)) != std::string::npos) && (idx < str.size()-1) ){
		ss << str.substr(last, (idx-last));
		std::string tmp(ss.str());
		switch(str.at(++idx)){
		case 'n':
			ss << "\n";
			break;
		case 'r':
			ss << "\r";
			break;
		case 't':
			ss << "\t";
			break;
		case 'b':
			ss << "\b";
			break;
		case 'f':
			ss << "\f";
			break;
		case '\"':
			ss << "\"";
			break;
		case '\'':
			ss << "\'";
			break;
		case '\\':
			ss << "\\";
			break;
		default:
			ss << "\\";
			ss << str.at(idx);
			break;
		}
		last = idx+1;
	}
	ss << str.substr(last);
	std::string result = ss.str();
	return result;
}
}}
