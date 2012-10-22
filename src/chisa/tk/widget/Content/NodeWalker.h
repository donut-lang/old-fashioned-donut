/*
 * ModelWalker.h
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#pragma once
#include <memory>
#include "Decl.h"

namespace chisa {
namespace tk {
namespace widget {

class NodeWalker {
public:
	NodeWalker() = default;
	virtual ~NodeWalker() = default;
	void walk(std::shared_ptr<Node> model);
	virtual void walk(Document* model) = 0;
	virtual void walk(Paragraph* model) = 0;
	virtual void walk(Heading* model) = 0;
	virtual void walk(Text* model) = 0;
};

}}}
