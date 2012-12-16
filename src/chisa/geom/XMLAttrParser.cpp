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

#include "Area.h"
#include "../util/XMLAttrParser.h"

namespace chisa {
namespace util {
namespace xml {

using namespace chisa::geom;

template <>
void parseAttr<Space>(std::string const& name, Space& v, Space const& def, tinyxml2::XMLElement* elm)
{
	float margin=0.0f;
	v=def;
	if(elm->QueryFloatAttribute(name.c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.margin(margin);
	}
	if(elm->QueryFloatAttribute((name+"-top").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.top(margin);
	}
	if(elm->QueryFloatAttribute((name+"-left").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.left(margin);
	}
	if(elm->QueryFloatAttribute((name+"-right").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.right(margin);
	}
	if(elm->QueryFloatAttribute((name+"-bottom").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.bottom(margin);
	}
}
}}}


