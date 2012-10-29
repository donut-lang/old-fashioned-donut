/**
 * Chisa
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

#include "../Color.h"
#include "../../util/XMLUtil.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace util {
namespace xml {

using namespace chisa::gl;

float __atof(const std::string& str, bool& failed)
{
	char* c = nullptr;
	int val = std::strtol(str.c_str(), &c, 16);
	failed |= !(c != nullptr && c[0] == '\0');
	if(str.length() <= 1){
		val = val | (val<<4);
	}
	return (val & 0xff)/255.0f;
}

template <>
void parseAttr<Color>(const std::string& name, Color& v, const Color& def, tinyxml2::XMLElement* elm)
{
	const char* const cv = elm->Attribute(name.c_str());
	if( !cv ){
		v = def;
		return;
	}
	if( cv[0] != '#' ){
		for(auto tbl : kColorTable){
			if(tbl.first == cv){
				v = tbl.second;
				break;
			}
		}
	}else{
		float r,g,b,a;
		bool failed = false;
		std::string color(&cv[1]);
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
			v=def;
		}else{
			v.alpha(a);
			v.red(r);
			v.blue(b);
			v.green(g);
		}
	}
}
}}}


