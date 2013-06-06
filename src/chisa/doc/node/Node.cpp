/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Node.h"

namespace chisa {
namespace doc {

Node::Node()
:root_(nullptr), block_(nullptr), parent_(nullptr)
{
}

std::shared_ptr<Document> Node::createRootDocument()
{
	std::shared_ptr<Document> node(new Document);
	node->root(node.get());
	node->block(nullptr);
	node->parent(nullptr);
	return node;
}


TreeNode::TreeNode()
{
	this->addAttribute("id", this->id_);
}

TreeNode::~TreeNode() noexcept
{
	for(Node* n : this->children_){
		delete n;
	}
	decltype(this->children_)().swap(this->children_);
}

TreeNode* TreeNode::findTreeNodeById(std::string const& name) noexcept
{
	if(!(this->id().empty()) && name == this->id()){
		return this;
	}
	for(Node* it : this->children_){
		TreeNode* const n = it->findTreeNodeById(name);
		if(n){
			return n;
		}
	}
	return nullptr;
}


Text* TreeNode::findFirstTextNode() noexcept
{
	for (Node* it : this->children_) {
		Text* const t = it->findFirstTextNode();
		if (t) {
			return t;
		}
	}
	return nullptr;
}


void TreeNode::parseAttribute(tinyxml2::XMLElement* node)
{
	for(auto it : this->attrMap_){
		//firstには要素名が、secondにはnodeをパースして設定する関数オブジェクトが入ってる
		it.second(node);
	}
}

BlockNode::BlockNode()
{
	this->margin(geom::Space(0,0,0,0));
	this->addAttribute("margin", this->margin_);
	this->direction(Direction::None);
	this->addAttribute("float", this->direction_);
	this->width(geom::Unspecified);
	this->addAttribute("width", this->width_);
	this->height(geom::Unspecified);
	this->addAttribute("height", this->height_);
	this->backgroundRepl("none:");
	this->addAttribute("background", this->backgroundRepl_);
}

Handler<RenderObject> BlockNode::background() const
{
	return this->background_;
}

void BlockNode::background(Handler<RenderObject> const& handler)
{
	this->background_ = handler;
}

InlineNode::InlineNode()
{

}

Document::Document()
{
	this->backgroundRepl("color:white");
}

Paragraph::Paragraph()
{

}

Heading::Heading(int const level)
:level_(level)
{
	//デフォルトマージンの設定
	switch(this->level()){
	case 1:
		this->margin(geom::Space(gl::TextDrawable::DefaultFontSize*1.5, gl::TextDrawable::DefaultFontSize, 0.0f,0.0f));
		break;
	case 2:
		this->margin(geom::Space(gl::TextDrawable::DefaultFontSize*1.25, gl::TextDrawable::DefaultFontSize*0.75, 0.0f,0.0f));
		break;
	case 3:
		this->margin(geom::Space(gl::TextDrawable::DefaultFontSize*1, gl::TextDrawable::DefaultFontSize*0.5, 0.0f,0.0f));
		break;
	default:
		CINAMO_EXCEPTION(Exception, "[BUG] Unknwon heading level: %d", this->level());
	}
}

BreakLine::BreakLine()
{
}

Link::Link()
{
	//this->addAttribute("src", )
}

Font::Font()
:size_(geom::Unspecified)
{
	this->color(gl::InvalidColor);
	this->addAttribute("color", this->color_);
	this->size(NAN);
	this->addAttribute("size", this->size_);
}

Text::Text(std::string text)
:text_(text)
{
}

Text* Text::findFirstTextNode() noexcept
{
	return this;
}
Text::DataType Text::objectAt(std::size_t idx) const noexcept{
	return this->objects_.at(idx);
}

void Text::clearObjects() {
	Text::ListType().swap(this->objects_);
}

void Text::appendObject( Text::DataType const& d ) {
	this->objects_.push_back( d );
}

}}

