/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Node.hpp"

namespace cinamo {
namespace xml {

using namespace chisa::doc;
template <> void parseAttr<BlockNode::Direction>(std::string const& name, BlockNode::Direction& v, BlockNode::Direction const& def, tinyxml2::XMLElement* elm)
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
}}
