/*
 * ModelReader.h
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#pragma once
#include "../../../util/ClassUtil.h"
#include <memory>
#include <map>
#include <string>
#include "Decl.h"

namespace tinyxml2 {
class XMLNode;
class XMLElement;
class XMLText;
}

namespace chisa {
namespace tk {
namespace widget {

class NodeReader {
	DISABLE_COPY_AND_ASSIGN(NodeReader);
private:
	static const std::string RootElementName;
private:
	typedef std::function<std::shared_ptr<Node>(NodeReader&, std::weak_ptr<Document>, std::weak_ptr<Node>, tinyxml2::XMLElement*)> ParseFunc;
	std::map<std::string, ParseFunc> elementParser_;
public:
	NodeReader();
	virtual ~NodeReader() = default;
public:
	std::shared_ptr<Document> parseTree(tinyxml2::XMLElement* elm);
	std::shared_ptr<Node> parseNode(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLNode* node);
private:
	std::shared_ptr<Node> parseText(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLText* txt);
	std::shared_ptr<Node> parseHeading(int level, std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLElement* elm);
};

}}}
