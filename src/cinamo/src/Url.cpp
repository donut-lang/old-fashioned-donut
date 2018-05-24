/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cinamo/Exception.h>
#include <cinamo/Url.h>
#include <cinamo/String.h>

namespace cinamo {
namespace url {
/**
 * Based upon
 * Uniform Resource Identifier (URI): Generic Syntax
 * 2.1. Percent-Encoding:
 * http://tools.ietf.org/html/rfc3986#section-2.1
 */

std::string decodePercent(std::string const& str)
{
	std::string to;
	std::string code;
	code.resize(2);
	const int max=str.length();
	for(std::string::const_iterator it = str.begin(); it != str.end(); ++it){
		switch( *it ) {
		case '%': {
			if( std::distance(str.begin(), it)+3 <= max ){
				code[0] = *(++it);
				code[1] = *(++it);
				char* failed = nullptr;
				const char c = static_cast<char>(std::strtol(code.c_str(), &failed, 16) & 0xff);
				if( (*failed) != '\0' ){
					CINAMO_EXCEPTION(Exception, "Failed to decode percent: %s", str.c_str());
				}
				to.push_back(c);
			}
			break;
		}
		default:
			to.push_back(*it);
			break;
		}
	}
	return to;
}

/**
 * application/x-www-form-urlencoded
 * http://tools.ietf.org/html/rfc1866#section-8.2.1
 */
std::string decodeForm(std::string const& str)
{
	std::string to;
	std::string code;
	code.resize(2);
	const int max=str.length();
	for(std::string::const_iterator it = str.begin(); it != str.end(); ++it){
		switch( *it ) {
		case '%': {
			if( std::distance(str.begin(), it)+3 <= max ){
				code[0] = *(++it);
				code[1] = *(++it);
				char* failed = nullptr;
				const char c = static_cast<char>(std::strtol(code.c_str(), &failed, 16) & 0xff);
				if( (*failed) != '\0' ){
					CINAMO_EXCEPTION(Exception, "Failed to decode percent: %s", str.c_str());
				}
				to.push_back(c);
			}
			break;
		}
		case '+': {
			to.push_back(' ');
			break;
		}
		default:
			to.push_back(*it);
			break;
		}
	}
	return to;
}

inline static bool isUnreserved( char const c )
{
	return std::isalnum(c) || c == '-' || c=='.' || c == '_' || c == '~';
}

inline static bool isGenDelim( char const c )
{
	return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@';
}

inline static bool isSubDelim( char const c )
{
	return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=';
}

std::string encodePercent(std::string const& str)
{
	std::string ret;
	ret.reserve(str.size()*2);

	for(char const& c : str){
		if( !isUnreserved(c) && !isGenDelim(c) && !isSubDelim(c) ) {
			ret.push_back('%');
			ret.append( format("%02X", static_cast<unsigned char>(c)) );
		}else{
			ret.push_back(c);
		}
	}

	return ret;
}

static bool shouldNotBeEncoded(char c){
	// http://tools.ietf.org/html/rfc1738
	// Thus, only alphanumerics, the special characters "$-_.+!*'(),", and
	// reserved characters used for their reserved purposes may be used
	// unencoded within a URL.
	return std::isalnum(c) || '$' == c || '-'==c || '_' == c || '.' == c || '+' == c || '!' == c || '*' == c || '\''== c || '(' == c || ')' == c || ',' == c;
}
std::string encodeForm(std::string const& str)
{
	std::string ret;
	ret.reserve(str.size()*2);

	for(char const& c : str){
		if( c == ' ' ) {
			ret.push_back('+');
		}else if( shouldNotBeEncoded(c) ) {
			ret.push_back(c);
		}else{
			ret.push_back('%');
			ret.append( format("%02X", static_cast<unsigned char>(c)) );
		}
	}

	return ret;
}

}}
