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
#include "../../../logging/Logger.h"
#include "../../../util/ClassUtil.h"
#include "Decl.h"
#include "NodeWalker.h"
#include <memory>
#include <vector>
#include <map>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace widget {

/******************************************************************************
 * 基底ノード
 ******************************************************************************/

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

#define NODE_SUBKLASS(Klass)\
protected:\
	Klass();\
NODE_SUBKLASS_DESTRUCTOR(Klass);\

#define NODE_SUBKLASS_LEAF(Klass)\
NODE_SUBKLASS_WALK(Klass);\
NODE_SUBKLASS_DESTRUCTOR(Klass);\
template <typename Derived, typename... Args>\
friend std::shared_ptr<Derived> Node::create(std::weak_ptr<Document> root, std::weak_ptr<Node> parent, const Args&... args)

/******************************************************************************
 * カテゴライズするためのノード
 ******************************************************************************/

class TreeNode : public Node {
	DEFINE_MEMBER(public, private, std::string, id);
	NODE_SUBKLASS(TreeNode);
private:
	DEFINE_MEMBER(public, private, std::vector<std::shared_ptr<Node> >, children);
	std::map<std::string, std::function<void(tinyxml2::XMLElement*)> > attrMap_;
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
	void parseAttribute(tinyxml2::XMLElement* elm);
protected:
	template <typename T> static void parseAttr(const std::string& name, T& ptr, const T& def, tinyxml2::XMLElement* elm);
	template <typename T> void addAttribute(const std::string& name, T& ptr, const T def=T())
	{
		using namespace std::placeholders;
		this->attrMap_.insert(std::make_pair(name, std::bind(TreeNode::parseAttr<T>, std::string(name), std::ref(ptr), def, _1)));
	}
};

class BlockNode : public TreeNode {
	NODE_SUBKLASS(BlockNode);
};

class InlineNode : public TreeNode {
	NODE_SUBKLASS(InlineNode);
};

/******************************************************************************
 * 実装ノード
 ******************************************************************************/

class Document : public TreeNode {
	NODE_SUBKLASS_LEAF(Document);
	friend std::shared_ptr<Document> Node::createRootDocument();
public:
	Document();
};

class Paragraph : public BlockNode {
	NODE_SUBKLASS_LEAF(Paragraph);
public:
	Paragraph();
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
