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

namespace chisa {
namespace util {
namespace xml {

using namespace chisa::tk::widget;
template <> void parseAttr<BlockNode::Direction>(const std::string& name, BlockNode::Direction& v, const BlockNode::Direction& def, tinyxml2::XMLElement* elm)
{
	if(const char* c = elm->Attribute(name.c_str())){
		std::string val(c);
		if(val == "right"){
			v = BlockNode::Direction::Right;
			return;
		}else if(val == "left"){
			v = BlockNode::Direction::Left;
			return;
		}else if(val == "none"){
			v = BlockNode::Direction::None;
			return;
		}
	}
	v = def;
}
}}}
