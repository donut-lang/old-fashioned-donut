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

#include <string>
#include <tarte/XMLAttrParser.h>

namespace tarte {
namespace xml {

template <> void parseAttr<int>(std::string const& name, int& v, int const& def, tinyxml2::XMLElement* elm)
{
	if(elm->QueryIntAttribute(name.c_str(), &v) != tinyxml2::XML_SUCCESS){
		v = def;
	}
}

template <> void parseAttr<float>(std::string const& name, float& v, float const& def, tinyxml2::XMLElement* elm)
{
	if(elm->QueryFloatAttribute(name.c_str(), &v) != tinyxml2::XML_SUCCESS){
		v = def;
	}
}

template <> void parseAttr<std::string>(std::string const& name, std::string& v, std::string const& def, tinyxml2::XMLElement* elm)
{
	if(const char* attr = elm->Attribute(name.c_str())){
		v = attr;
	}else{
		v = def;
	}
}

template <> void parseAttr<unsigned int>(std::string const& name, unsigned int& v, const unsigned int& def, tinyxml2::XMLElement* elm)
{
	if(elm->QueryUnsignedAttribute(name.c_str(), &v) != tinyxml2::XML_SUCCESS){
		v = def;
	}
}

template <> void parseAttr<bool>(std::string const& name, bool& v, bool const& def, tinyxml2::XMLElement* elm)
{
	if(elm->QueryBoolAttribute(name.c_str(), &v) != tinyxml2::XML_SUCCESS){
		v = def;
	}
}

}}
