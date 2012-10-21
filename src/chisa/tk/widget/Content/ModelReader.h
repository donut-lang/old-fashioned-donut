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

class ModelReader {
	DISABLE_COPY_AND_ASSIGN(ModelReader);
private:
	typedef std::function<std::shared_ptr<Model>(ModelReader&, std::weak_ptr<Model>, std::weak_ptr<Model>, tinyxml2::XMLElement*)> ParseFunc;
	std::map<std::string, ParseFunc> elementParser_;
public:
	ModelReader();
	virtual ~ModelReader() = default;
public:
	std::shared_ptr<Model> parseTree(std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLNode* node);
	std::shared_ptr<Model> parseTree(tinyxml2::XMLNode* node);
private:
	std::shared_ptr<Model> parseText(std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLText* txt);
	std::shared_ptr<Model> parseHeading(int level, std::weak_ptr<Model> root, std::weak_ptr<Model> parent, tinyxml2::XMLElement* elm);
};

}}}
