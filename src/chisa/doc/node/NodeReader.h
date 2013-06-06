/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/VectorMap.h>
#include <memory>
#include <string>
#include <tinyxml2.h>
#include "Decl.h"

namespace chisa {
using namespace cinamo;

namespace doc {

class NodeReader {
	DISABLE_COPY_AND_ASSIGN(NodeReader);
public:
	typedef std::function<Node*(NodeReader&, Document*, BlockNode*, TreeNode*, tinyxml2::XMLElement*)> ParseFunc;
	typedef std::function<TreeNode*(Document*, BlockNode*, TreeNode*)> TreeConstructor;
	typedef std::function<BlockNode*(Document*, BlockNode*, TreeNode*)> BlockConstructor;
private:
	static const std::string RootElementName;
	VectorMap<std::string, ParseFunc> elementParser_;
public:
	NodeReader();
	virtual ~NodeReader() = default;
public:
	std::shared_ptr<Document> parseTree(tinyxml2::XMLElement* elm);
	Node* parseNode(Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLNode* node);
	TreeNode* parseTreeNode(TreeConstructor constructor, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm);
	BlockNode* parseBlockNode(BlockConstructor constructor, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm);
private:
	Node* parseText(Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLText* txt);
};

}}
