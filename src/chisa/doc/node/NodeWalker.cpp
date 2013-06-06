/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Node.h"
#include "NodeWalker.h"

namespace chisa {
namespace doc {

NodeWalker::NodeWalker() noexcept
:nodeDepth_(0)
{

}

void NodeWalker::start(std::shared_ptr<Document> model)
{
	model->walk(*this);
}

void NodeWalker::walkChildren(TreeNode* children)
{
	this->nodeDepth_++;
	for(Node* c : *children){
		c->walk(*this);
	}
	this->nodeDepth_--;
}


}}
