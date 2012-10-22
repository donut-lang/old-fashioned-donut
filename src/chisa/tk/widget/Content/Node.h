/*
 * Model.h
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#pragma once
#include "../../../logging/Logger.h"
#include "../../../util/ClassUtil.h"
#include "Decl.h"
#include "NodeWalker.h"
#include <memory>
#include <vector>

namespace chisa {
namespace tk {
namespace widget {

class Node {
	DISABLE_COPY_AND_ASSIGN(Node);
	DEFINE_MEMBER(public, private, std::weak_ptr<Document>, root);
	DEFINE_MEMBER(public, private, std::weak_ptr<Node>, parent);
protected:
	Node();
public:
	virtual ~Node() noexcept = default;
public:
	virtual void walked(NodeWalker& walker) = 0;
	template <typename Derived, typename... Args>
	static std::shared_ptr<Derived> create(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, const Args&... args)
	{
		std::shared_ptr<Derived> node(new Derived(args...));
		node->root(root);
		node->parent(parent);
		return node;
	}
	static std::shared_ptr<Document> createRootDocument();
};

#define NODE_SUBKLASS_DESTRUCTOR(Klass) public: virtual ~Klass() noexcept = default;
#define NODE_SUBKLASS_WALK(Klass) 	public: virtual void walked(NodeWalker& walker) override { walker.walk(this); };

#define NODE_SUBKLASS_LEAF(Klass)\
NODE_SUBKLASS_DESTRUCTOR(Klass);\
NODE_SUBKLASS_WALK(Klass);\
template <typename Derived, typename... Args>\
friend std::shared_ptr<Derived> Node::create(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, const Args&... args)

#define NODE_SUBKLASS_WITH_IMPL_LEAF(Klass)\
private: Klass(){}\
NODE_SUBKLASS_LEAF(Klass);

#define NODE_SUBKLASS_WITH_IMPL(Klass)\
protected: Klass(){}\
NODE_SUBKLASS_DESTRUCTOR(Klass);

class TreeNode : public Node {
	NODE_SUBKLASS_WITH_IMPL(TreeNode);
private:
	std::vector<std::shared_ptr<Node> > children_;\
protected:
	inline std::vector<std::shared_ptr<Node> >& children() { return children_; }
	inline const std::vector<std::shared_ptr<Node> >& children() const { return children_; }
public:
	typedef std::vector<std::shared_ptr<Node> >::iterator Iterator;
	typedef std::vector<std::shared_ptr<Node> >::const_iterator ConstIterator;
public:
	inline Iterator begin() { return this->children_.begin(); };
	inline Iterator end() { return this->children_.end(); };
	inline ConstIterator cbegin() const { return this->children_.cbegin(); };
	inline ConstIterator cend() const { return this->children_.cend(); };
	inline void add(std::shared_ptr<Node> child) { this->children_.push_back(child); };
	inline size_t count() const { return this->children_.size(); };
	inline std::shared_ptr<Node> at(size_t idx) const { return this->children_.at(idx); };
};

class BlockNode : public TreeNode {
	NODE_SUBKLASS_WITH_IMPL(BlockNode);
};

class InlineNode : public TreeNode {
	NODE_SUBKLASS_WITH_IMPL(InlineNode);
};

class Document : public TreeNode {
	NODE_SUBKLASS_WITH_IMPL_LEAF(Document);
	friend std::shared_ptr<Document> Node::createRootDocument();
};

class Paragraph : public BlockNode {
	NODE_SUBKLASS_WITH_IMPL_LEAF(Paragraph);
};

class Heading : public BlockNode {
	DEFINE_MEMBER_CONST(public, int, level);
	NODE_SUBKLASS_LEAF(Heading);
private:
	Heading(int const level);
};

class Text : public Node {
	DEFINE_MEMBER(public, private, std::string, text);
	NODE_SUBKLASS_LEAF(Text);
private:
	Text(std::string text);
};

}}}
