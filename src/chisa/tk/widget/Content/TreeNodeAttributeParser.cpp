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
#include "Node.h"
#include "Margin.h"

namespace chisa {
namespace tk {
namespace widget {

template <> void TreeNode::parseAttr<int>(const std::string& name, int& v, const int& def, tinyxml2::XMLElement* elm)
{
	if(!elm->QueryIntAttribute(name.c_str(), &v)){
		v = def;
	}
}

template <> void TreeNode::parseAttr<float>(const std::string& name, float& v, const float& def, tinyxml2::XMLElement* elm)
{
	if(!elm->QueryFloatAttribute(name.c_str(), &v)){
		v = def;
	}
}

template <> void TreeNode::parseAttr<std::string>(const std::string& name, std::string& v, const std::string& def, tinyxml2::XMLElement* elm)
{
	if(const char* attr = elm->Attribute(name.c_str())){
		v = attr;
	}else{
		v = def;
	}
}

template <> void TreeNode::parseAttr<unsigned int>(const std::string& name, unsigned int& v, const unsigned int& def, tinyxml2::XMLElement* elm)
{
	if(!elm->QueryUnsignedAttribute(name.c_str(), &v)){
		v = def;
	}
}

template <> void TreeNode::parseAttr<bool>(const std::string& name, bool& v, const bool& def, tinyxml2::XMLElement* elm)
{
	if(!elm->QueryBoolAttribute(name.c_str(), &v)){
		v = def;
	}
}

template <> void TreeNode::parseAttr<Margin>(const std::string& name, Margin& v, const Margin& def, tinyxml2::XMLElement* elm)
{
	float margin;
	if(elm->QueryFloatAttribute(name.c_str(), &margin)){
		v.margin(margin);
	}
	if(elm->QueryFloatAttribute((name+"-top").c_str(), &margin)){
		v.top(margin);
	}
	if(elm->QueryFloatAttribute((name+"-left").c_str(), &margin)){
		v.left(margin);
	}
	if(elm->QueryFloatAttribute((name+"-right").c_str(), &margin)){
		v.right(margin);
	}
	if(elm->QueryFloatAttribute((name+"-bottom").c_str(), &margin)){
		v.bottom(margin);
	}
}

}}}
