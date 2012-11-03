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
#include <memory>
#include <vector>
#include <map>
#include <tinyxml2.h>
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include "../../util/XMLUtil.h"
#include "../../geom/Area.h"
#include "../../gl/Color.h"
#include "../../gl/Drawable.h"
#include "Decl.h"
#include "NodeWalker.h"
#include "../render/RenderObject.h"

namespace chisa {
namespace doc {

/******************************************************************************
 * 基底ノード
 ******************************************************************************/

class Node {
	DISABLE_COPY_AND_ASSIGN(Node);
	DEFINE_MEMBER(public, private, Document*, root);
	DEFINE_MEMBER(public, private, BlockNode*, block);
	DEFINE_MEMBER(public, private, TreeNode*, parent);
protected:
	Node();
public:
	virtual TreeNode* findTreeNodeById(const std::string& name) noexcept{ return nullptr; };
	virtual Text* findFirstTextNode() noexcept { return nullptr; };
public: //オブジェクトの生成
	virtual void walk(NodeWalker& walker) = 0;
	template <typename Derived, typename... Args>
	static Derived* create(Document* root, BlockNode* block, TreeNode* parent, const Args&... args)
	{
		Derived* const node = new Derived(args...);
		node->root(root);
		node->block(block);
		node->parent(parent);
		return node;
	}
	static std::shared_ptr<Document> createRootDocument();
	virtual ~Node() noexcept = default;
};

#define NODE_SUBKLASS_DESTRUCTOR(Klass) public: virtual ~Klass() noexcept = default;
#define NODE_SUBKLASS_WALK(Klass) public: virtual void walk(NodeWalker& walker) override { walker.walk(this); };

#define NODE_SUBKLASS(Klass)\
protected:\
	Klass();\
NODE_SUBKLASS_DESTRUCTOR(Klass);

#define NODE_SUBKLASS_LEAF(Klass)\
NODE_SUBKLASS_WALK(Klass);\
NODE_SUBKLASS_DESTRUCTOR(Klass);\
template <typename Derived, typename... Args>\
friend Derived* Node::create(Document* root, BlockNode* block, TreeNode* parent, const Args&... args)

/******************************************************************************
 * カテゴライズするためのノード
 ******************************************************************************/

class TreeNode : public Node {
public:
	typedef std::vector<Node*> ChildrenType;
	typedef ChildrenType::iterator Iterator;
	typedef ChildrenType::const_iterator ConstIterator;
private:
	DEFINE_MEMBER(public, private, std::string, id);
private:
	DEFINE_MEMBER(public, private, ChildrenType, children);
	std::map<std::string, std::function<void(tinyxml2::XMLElement*)> > attrMap_;
public:
	inline Iterator begin() { return this->children_.begin(); };
	inline Iterator end() { return this->children_.end(); };
	inline ConstIterator cbegin() const { return this->children_.cbegin(); };
	inline ConstIterator cend() const { return this->children_.cend(); };
	inline void add(Node* child) { this->children_.push_back(child); };
	inline size_t count() const { return this->children_.size(); };
	inline Node* at(size_t idx) const { return this->children_.at(idx); };
	void parseAttribute(tinyxml2::XMLElement* elm);
	virtual TreeNode* findTreeNodeById(const std::string& name) noexcept override;
	virtual Text* findFirstTextNode() noexcept override;
protected:
	template <typename T> void addAttribute(const std::string& name, T& ptr)
	{
		this->attrMap_.insert(std::make_pair(name, std::bind(chisa::util::xml::parseAttr<T>, std::string(name), std::ref(ptr), std::ref(ptr), std::placeholders::_1)));
	}
protected:
	TreeNode();
public:
	virtual ~TreeNode() noexcept;
};

class BlockNode : public TreeNode {
public:
	enum Direction {
		None,
		Right,
		Left
	};
	DEFINE_MEMBER(public, private, geom::Margin, margin);
	DEFINE_MEMBER(public, private, Direction, direction);
	DEFINE_MEMBER(public, private, float, width);
	DEFINE_MEMBER(public, private, float, height);
	DEFINE_MEMBER(public, protected, std::string, backgroundRepl);
private:
	Handler<RenderObject> background_;
public:
	Handler<RenderObject> background() const;
	void background(const Handler<RenderObject>& handler);
private:
	NODE_SUBKLASS(BlockNode);

};

class InlineNode : public TreeNode {
	NODE_SUBKLASS(InlineNode);
};

/******************************************************************************
 * 実装ノード
 ******************************************************************************/

class Document : public BlockNode {
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

class BreakLine : public BlockNode {
	NODE_SUBKLASS_LEAF(BreakLine);
private:
	BreakLine();
};

class Link : public InlineNode {
	NODE_SUBKLASS_LEAF(Link);
public:
	Link();
};

class Font : public InlineNode {
	NODE_SUBKLASS_LEAF(Font);
private:
	DEFINE_MEMBER(public, private, gl::Color, color);
	DEFINE_MEMBER(public, private, float, size);
public:
	Font();
};

class Text : public Node {
public:
	typedef Handler<RenderObject> DataType;
	typedef std::vector<DataType> ListType;
private:
	DEFINE_MEMBER(public, private, std::string, text);
	NODE_SUBKLASS_LEAF(Text);
private:
	ListType objects_;
private:
	Text(std::string text);
public:
	void clearObjects();
	void appendObject( const DataType& d );
	DataType objectAt(std::size_t idx) const noexcept;
	inline std::size_t objectCount() const noexcept{ return this->objects_.size(); };
	inline ListType const& objects() const noexcept { return this->objects_; }
public:
	virtual Text* findFirstTextNode() noexcept override;
};

#undef NODE_SUBKLASS_DESTRUCTOR
#undef NODE_SUBKLASS_WALK
#undef NODE_SUBKLASS
#undef NODE_SUBKLASS_LEAF

}}
