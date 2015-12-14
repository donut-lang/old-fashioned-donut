/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Exception.h>
#include <tinyxml2.h>

#include "NodeReader.hpp"
#include "Node.hpp"

namespace chisa {
namespace doc {

const std::string NodeReader::RootElementName("doc");

template <typename T, typename U=void>
struct Proxy {
private:
	NodeReader::TreeConstructor c;
public:
	template <typename... Args>
	Proxy(Args const&... args)
	:c( std::bind(Node::create<T, Args...>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, args...) )
	{
	}
	Node* operator()(NodeReader& reader, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm) const
	{
		return reader.parseTreeNode(c, root, block, parent, elm);
	}
};

template <typename T>
struct Proxy<T, typename std::enable_if<std::is_base_of<BlockNode, T>::value>::type> {
private:
	NodeReader::BlockConstructor c;
public:
	template <typename... Args>
	Proxy(Args const&... args)
	:c( std::bind(Node::create<T, Args...>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, args...) )
	{
	}
	Node* operator()(NodeReader& reader, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm) const
	{
		return reader.parseBlockNode(c, root, block, parent, elm);
	}
};

NodeReader::NodeReader()
{
	using namespace std::placeholders;
	this->elementParser_.insert("h1", Proxy<Heading>(1));
	this->elementParser_.insert("h2", Proxy<Heading>(2));
	this->elementParser_.insert("h3", Proxy<Heading>(3));
	this->elementParser_.insert("p", Proxy<Paragraph>());
	this->elementParser_.insert("br", Proxy<BreakLine>());
	this->elementParser_.insert("font", Proxy<Font>());
	this->elementParser_.insert("link", Proxy<Link>());
}

#define NODE_FOREACH(it, node)\
for(auto it = (node)->FirstChild(); it; it = it->NextSibling())

std::shared_ptr<Document> NodeReader::parseTree(tinyxml2::XMLElement* elm)
{
	if(!elm){
		CINAMO_EXCEPTION(Exception, "[BUG] parsing null element.");
	}
	if(RootElementName != elm->Name()){
		CINAMO_EXCEPTION(Exception, "Invalid document tag: %s",elm->Name());
	}
	std::shared_ptr<Document> doc(Node::createRootDocument());
	doc->parseAttribute(elm);
	NODE_FOREACH(it, elm) {
		doc->add(this->parseNode(doc.get(), doc.get(), doc.get(), it));
	}
	return doc;
}

Node* NodeReader::parseNode(Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLNode* node)
{
	if(!node){
		CINAMO_EXCEPTION(Exception, "[BUG] parsing null node!");
	}
	if(tinyxml2::XMLElement* elm = node->ToElement()){
		auto it = this->elementParser_.find(elm->Name());
		if(it == this->elementParser_.end()){
			CINAMO_EXCEPTION(Exception, "Unknwon document tag: %s", elm->Name());
		}
		VectorMap<std::string, ParseFunc>::Pair const& p = *it;
		const ParseFunc f = p.second;
		return f(*this, root, block, parent, elm);
	}else if(tinyxml2::XMLText* txt = node->ToText()){
		return this->parseText(root, block, parent, txt);
	}
	CINAMO_EXCEPTION(Exception, "Unknwon document node: %s", node->Value());
}

TreeNode* NodeReader::parseTreeNode(TreeConstructor constructor, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm)
{
	TreeNode* tree = constructor(root, block, parent);
	tree->parseAttribute(elm);
	NODE_FOREACH(it, elm) {
		tree->add(this->parseNode(root, block, tree, it));
	}
	return tree;
}

BlockNode* NodeReader::parseBlockNode(BlockConstructor constructor, Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLElement* elm)
{
	BlockNode* tree = constructor(root, block, parent);
	tree->parseAttribute(elm);
	NODE_FOREACH(it, elm) {
		tree->add(this->parseNode(root, tree, tree, it));
	}
	return tree;
}

Node* NodeReader::parseText(Document* root, BlockNode* block, TreeNode* parent, tinyxml2::XMLText* txt)
{
	return Node::create<Text>(root, block, parent, txt->Value());
}

}}
