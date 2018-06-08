/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <memory>
#include <vector>
#include <tinyxml2.h>
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/XMLAttrParser.h>
#include <cinamo/VectorMap.h>
#include "../../geom/Area.hpp"
#include "../../gl/Color.hpp"
#include "../../gl/Drawable.hpp"
#include "Decl.hpp"
#include "NodeWalker.hpp"
#include "../render/RenderObject.hpp"

namespace chisa {
using namespace cinamo;

namespace doc {

/******************************************************************************
 * 基底ノード
 ******************************************************************************/

class Node {
	DISABLE_COPY_AND_ASSIGN(Node);
	DEFINE_MEMBER(public, protected, Document*, root);
	DEFINE_MEMBER(public, protected, BlockNode*, block);
	DEFINE_MEMBER(public, protected, TreeNode*, parent);
protected:
	Node();
public:
	virtual TreeNode* findTreeNodeById(std::string const& name) noexcept{ return nullptr; };
	virtual Text* findFirstTextNode() noexcept { return nullptr; };
public: //オブジェクトの生成
	virtual void walk(NodeWalker& walker) = 0;
	template <typename Derived, typename... Args>
	static Derived* create(Document* root, BlockNode* block, TreeNode* parent, Args const&... args)
	{
		Derived* const node = new Derived(args...);
		node->root(root);
		node->block(block);
		node->parent(parent);
		return node;
	}
	static std::shared_ptr<Document> createRootDocument();
	virtual ~Node() noexcept = default;
public:
	inline bool isRoot() const noexcept { return parent_ == nullptr; };
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
friend Derived* Node::create(Document* root, BlockNode* block, TreeNode* parent, Args const&... args)

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
	VectorMap<std::string, std::function<void(tinyxml2::XMLElement*)> > attrMap_;
public: /* as component */
	inline Iterator begin() { return this->children_.begin(); };
	inline Iterator end() { return this->children_.end(); };
	inline ConstIterator cbegin() const { return this->children_.cbegin(); };
	inline ConstIterator cend() const { return this->children_.cend(); };
	inline void add(Node* child) { this->children_.push_back(child); };
	inline size_t count() const { return this->children_.size(); };
	inline Node* at(size_t idx) const { return this->children_.at(idx); };
public:
	void parseAttribute(tinyxml2::XMLElement* elm);
	virtual TreeNode* findTreeNodeById(std::string const& name) noexcept override;
	virtual Text* findFirstTextNode() noexcept override;
protected:
	template <typename T> void addAttribute(std::string const& name, T& ptr)
	{
		this->attrMap_.insert(name, std::bind(xml::parseAttr<T>, std::string(name), std::ref(ptr), std::ref(ptr), std::placeholders::_1));
	}
protected:
	TreeNode();
public:
	virtual ~TreeNode() noexcept;
};

class BlockNode : public TreeNode {
public:
	enum Direction {
		Unspecified,
		None,
		Right,
		Left
	};
	DEFINE_MEMBER(public, public, geom::Area, areaInBlock);
	DEFINE_MEMBER(public, public, geom::Area, area);
	DEFINE_MEMBER(public, protected, geom::Space, margin);
	DEFINE_MEMBER(public, private, Direction, direction);
	DEFINE_MEMBER(public, private, float, width);
	DEFINE_MEMBER(public, private, float, height);
	DEFINE_MEMBER(public, protected, std::string, backgroundRepl);
private:
	Handler<RenderObject> background_;
public:
	Handler<RenderObject> background() const;
	void background(Handler<RenderObject> const& handler);
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
	typedef Handler<TextDrawableObject> DataType;
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
	void appendObject( DataType const& d );
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
