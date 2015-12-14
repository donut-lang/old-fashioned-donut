/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <string>
#include <cstdlib>
#include <sstream>
#include <antlr3.h>
#include "ParseUtil.hpp"

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

const std::string unescapeString(std::string const& str){
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
}
