/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <string>
#include <cinamo/XMLAttrParser.h>
#include <cinamo/String.h>

namespace cinamo {
namespace xml {

#define PARSE_AS(type)\
template <> void parseAttr<type>(std::string const& name, type& v, type const& def, tinyxml2::XMLElement* elm)\
{\
	const char* attr = elm->Attribute(name.c_str());\
	v = attr ? parseAs<type>(attr, def) : def;\
}

PARSE_AS(int);
PARSE_AS(unsigned int);
PARSE_AS(unsigned long long);
PARSE_AS(long long);
PARSE_AS(float);
PARSE_AS(bool);

#undef PARSE_AS

template <> void parseAttr<std::string>(std::string const& name, std::string& v, std::string const& def, tinyxml2::XMLElement* elm)
{
	if(const char* attr = elm->Attribute(name.c_str())){
		v = attr;
	}else{
		v = def;
	}
}

}}
