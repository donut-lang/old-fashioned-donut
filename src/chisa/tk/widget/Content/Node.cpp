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

std::shared_ptr<Document> Node::createRootDocument()
{
	std::shared_ptr<Document> node(new Document);
	node->root(node);
	node->parent(std::weak_ptr<Node>());
	return node;
}


}}}
