/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <cstdlib>

#include "Color.h"

namespace chisa {
namespace gl {
float __atof(std::string const& str, bool& failed)
{
	char* c = nullptr;
	int val = std::strtol(str.c_str(), &c, 16);
	failed |= !(c != nullptr && c[0] == '\0');
	if(str.length() <= 1){
		val = val | (val<<4);
	}
	return (val & 0xff)/255.0f;
}

Color Color::fromString(std::string const& name)
{
	if( name.empty() ){
		return InvalidColor;
	}
	if( name[0] != '#' ){
		for(auto tbl : COLOR_TABLE){
			if(tbl.first == name){
				return tbl.second;
				break;
			}
		}
		return InvalidColor;
	}else{
		float r,g,b,a;
		bool failed = false;
		std::string color(name.substr(1));
		switch( color.size() ){
		case 3: //RGB
			r = __atof( color.substr(0,1), failed );
			g = __atof( color.substr(1,1), failed );
			b = __atof( color.substr(2,1), failed );
			a = 1;
			break;
		case 4: //RGBA
			r = __atof( color.substr(0,1), failed );
			g = __atof( color.substr(1,1), failed );
			b = __atof( color.substr(2,1), failed );
			a = __atof( color.substr(3,1), failed );
			break;
		case 6: //RRGGBB
			r = __atof( color.substr(0,2), failed );
			g = __atof( color.substr(2,2), failed );
			b = __atof( color.substr(4,2), failed );
			a=1;
			break;
		case 8: //RRGGBBAA
			r = __atof( color.substr(0,2), failed );
			g = __atof( color.substr(2,2), failed );
			b = __atof( color.substr(4,2), failed );
			a = __atof( color.substr(6,2), failed );
			break;
		default:
			failed = true;
			break;
		}
		if(failed){
			return InvalidColor;
		}else{
			return Color(r,g,b,a);
		}
	}
}

std::string Color::toString() const noexcept
{
	return ::cinamo::format("(Color %p r:%0.2f g:%0.2f b:%0.2f a:%0.2f)", this, red_, green_, blue_, alpha_);
}
}}
