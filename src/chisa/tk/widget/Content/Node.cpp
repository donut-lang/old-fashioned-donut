/*
 * Model.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#include "Node.h"

namespace chisa {
namespace tk {
namespace widget {

Node::Node()
{
}

TreeNode::TreeNode()
{
	this->addAttribute<std::string>("id", this->id_);
}

BlockNode::BlockNode()
{

}

InlineNode::InlineNode()
{

}

void TreeNode::parseAttribute(tinyxml2::XMLElement* node)
{
	for(auto it : this->attrMap_){
		it.second(node);
	}
}

std::shared_ptr<Document> Node::createRootDocument()
{
	std::shared_ptr<Document> node(new Document);
	node->root(node);
	node->parent(std::weak_ptr<Node>());
	return node;
}

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


Heading::Heading(int const level)
:BlockNode()
,level_(level)
{
}

Text::Text(std::string text)
:Node()
,text_(text)
{
}

}}}
