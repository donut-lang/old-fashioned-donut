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
public:
	typedef std::function<std::shared_ptr<Node>(NodeReader&, std::weak_ptr<Document>, std::weak_ptr<Node>, tinyxml2::XMLElement*)> ParseFunc;
	typedef std::function<std::shared_ptr<TreeNode>(std::weak_ptr<Document>, std::weak_ptr<Node>)> TreeConstructor;
private:
	static const std::string RootElementName;
	std::map<std::string, ParseFunc> elementParser_;
public:
	NodeReader();
	virtual ~NodeReader() = default;
public:
	std::shared_ptr<Document> parseTree(tinyxml2::XMLElement* elm);
	std::shared_ptr<Node> parseNode(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLNode* node);
private:
	std::shared_ptr<TreeNode> parseTreeNode(TreeConstructor constructor, std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLElement* elm);
	std::shared_ptr<Node> parseText(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, tinyxml2::XMLText* txt);
};

}}}
