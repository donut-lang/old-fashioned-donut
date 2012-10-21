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

class ModelWalker {
public:
	ModelWalker() = default;
	virtual ~ModelWalker() = default;
	void walk(std::shared_ptr<Model> model);
	virtual void walk(Paragraph* model) = 0;
	virtual void walk(Heading* model) = 0;
	virtual void walk(Text* model) = 0;
};

}}}
