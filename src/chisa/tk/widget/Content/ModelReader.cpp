/*
 * ModelReader.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#include "ModelReader.h"
#include "../../../logging/Exception.h"
#include <tinyxml2.h>
#include "Model.h"

namespace chisa {
namespace tk {
namespace widget {

ModelReader::ModelReader()
{
	using namespace std::placeholders;
	this->elementParser_.insert(std::make_pair("h1", std::bind(&ModelReader::parseHeading, _1,1,_2, _3, _4)));
	this->elementParser_.insert(std::make_pair("h2", std::bind(&ModelReader::parseHeading, _1,2,_2, _3, _4)));
	this->elementParser_.insert(std::make_pair("h3", std::bind(&ModelReader::parseHeading, _1,3,_2, _3, _4)));
}

std::shared_ptr<Model> ModelReader::parseTree(tinyxml2::XMLNode* node)
{
	return this->parseTree(std::weak_ptr<Model>(), std::weak_ptr<Model>(), node);
}

std::shared_ptr<Model> ModelReader::parseTree(std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLNode* node)
{
	if(root.expired() && !parent.expired()){
		root = parent;
	}
	if(tinyxml2::XMLDocument* doc = node->ToDocument()){
		node = doc->RootElement();
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

std::shared_ptr<Model> ModelReader::parseText(std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLText* txt)
{
	return std::shared_ptr<Model>(new Text(root, parent, txt->Value()));
}

#define NODE_FOREACH(it, node)\
for(auto it = (node)->FirstChild(); it; it = it->NextSibling())

std::shared_ptr<Model> ModelReader::parseHeading(int level, std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLElement* elm)
{
	std::shared_ptr<Heading> model(new Heading(root, parent, level));
	NODE_FOREACH(it, elm) {
		model->add(this->parseTree(root, model, it));
	}
	return model;
}

}}}
