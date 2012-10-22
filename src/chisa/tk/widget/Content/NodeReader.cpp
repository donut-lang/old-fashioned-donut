/*
 * ModelReader.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#include "NodeReader.h"
#include "../../../logging/Exception.h"
#include <tinyxml2.h>
#include "Node.h"

namespace chisa {
namespace tk {
namespace widget {

const std::string NodeReader::RootElementName("doc");

NodeReader::NodeReader()
{
	using namespace std::placeholders;
	this->elementParser_.insert(std::make_pair("h1", std::bind(&NodeReader::parseHeading, _1,1,_2, _3, _4)));
	this->elementParser_.insert(std::make_pair("h2", std::bind(&NodeReader::parseHeading, _1,2,_2, _3, _4)));
	this->elementParser_.insert(std::make_pair("h3", std::bind(&NodeReader::parseHeading, _1,3,_2, _3, _4)));
}

#define NODE_FOREACH(it, node)\
for(auto it = (node)->FirstChild(); it; it = it->NextSibling())

std::shared_ptr<Document> NodeReader::parseTree(tinyxml2::XMLElement* elm)
{
	if(!elm){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] parsing null element.");
	}
	if(RootElementName != elm->Name()){
		throw logging::Exception(__FILE__, __LINE__, "Invalid document tag: %s",elm->Name());
	}
	std::shared_ptr<Document> doc(Node::createRootDocument());
	NODE_FOREACH(it, elm) {
		doc->add(this->parseNode(doc, doc, it));
	}
	return doc;
}

std::shared_ptr<Node> NodeReader::parseNode(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLNode* node)
{
	if(!node){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] parsing null node!");
	}
	if(tinyxml2::XMLElement* elm = node->ToElement()){
		auto it = this->elementParser_.find(elm->Name());
		if(it == this->elementParser_.end()){
			throw logging::Exception(__FILE__, __LINE__, "Unknwon document tag: %s", elm->Name());
		}
		const ParseFunc f = it->second;
		return f(*this, root, parent, elm);
	}else if(tinyxml2::XMLText* txt = node->ToText()){
		return this->parseText(root, parent, txt);
	}
	throw logging::Exception(__FILE__, __LINE__, "Unknwon document node: %s", node->Value());
}

std::shared_ptr<Node> NodeReader::parseText(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLText* txt)
{
	return std::shared_ptr<Node>(Node::create<Text>(root, parent, txt->Value()));
}

std::shared_ptr<Node> NodeReader::parseHeading(int level, std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLElement* elm)
{
	std::shared_ptr<Heading> model(Node::create<Heading>(root, parent, level));
	NODE_FOREACH(it, elm) {
		model->add(this->parseNode(root, model, it));
	}
	return model;
}

}}}
