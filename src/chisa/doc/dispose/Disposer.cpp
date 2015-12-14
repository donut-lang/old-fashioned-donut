/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Disposer.hpp"
#include "BlockSession.hpp"
#include "../Util.hpp"
#include "../render/RenderObject.hpp"
#include "../dispose/PostDisposer.hpp"

namespace chisa {
namespace doc {

const static std::string TAG("Disposer");

Disposer::Disposer(Logger& log, Handler<RenderTree> renderTree, geom::Box const& constraint)
:log_(log)
,renderTree_(renderTree)
,dmanager_(renderTree->drawableManager())
,widgetSize_(constraint)
,textContext_(log, renderTree_)
,nowSession_(nullptr)
,nowDepth_(0.0f)
{
}

BlockSession* Disposer::nowSession()
{
	if(!this->nowSession_){
		this->log().w(TAG, "oops. nowSession() called, but now session is nullptr.");
	}
	return this->nowSession_;
}
void Disposer::rewriteSession(BlockSession* session)
{
	this->nowSession_ = session;
}

geom::Box Disposer::start(std::shared_ptr<Document> doc)
{
	this->renderTree_->reset();
	BlockSession ss(this, widgetSize_);
	this->NodeWalker::start(doc);
	doc::PostDisposer(log()).start(doc);
	return doc->areaInBlock().box();
}

template <typename T, typename U>
void Disposer::walkBlock(T* block, U clos)
{
	Handler<RenderObject> background(
			new NormalDrawableObject(HandlerW<RenderTree>(this->renderTree_), block, this->nowDepth_, block->backgroundRepl(), geom::Box())
	);
	block->background(background);
	this->renderTree_->addObject(background);
	this->nowDepth_+=0.1;
	{
		BlockSession bs(this, block);
		clos();
	}
	this->nowDepth_-=0.1;
}

void Disposer::walk(Document* doc)
{
	this->walkBlock(doc, [&](){
		this->walkChildren(doc);
	});
}

void Disposer::walk(Paragraph* para)
{
	this->walkBlock(para, [&](){
		this->walkChildren(para);
	});
}

void Disposer::walk(Heading* heading)
{
	this->walkBlock(heading, [&](){
		this->textContext_.pushSize(gl::TextDrawable::DefaultFontSize*(1.0f+heading->level()/2.0f));
		this->walkChildren(heading);
		this->textContext_.popSize();
	});
}

void Disposer::walk(Link* link)
{
	this->walkChildren(link);
}

void Disposer::walk(Font* font)
{
	if(!font->color().isInvalid()){
		this->textContext_.pushColor(font->color());
	}
	if(font->size() > 0){
		this->textContext_.pushSize(font->size());
	}
	this->walkChildren(font);
	if(!font->color().isInvalid()){
		this->textContext_.popColor();
	}
	if(font->size() > 0){
		this->textContext_.popSize();
	}
}

void Disposer::walk(BreakLine* br)
{
	//改行するだけ
	this->newInline();
}

void Disposer::walk(Text* text)
{
	text->clearObjects();
	std::vector<std::string> lines(breakLine(shrinkSpace(text->text())));
	for(std::string const& str : lines){
		Handler<TextDrawableObject> obj = this->textContext_.create(str, text, this->nowDepth_);
		//文字分のエリアを確保し、その位置とレンダリングコマンドを記録
		this->extendInline(obj);
		text->appendObject(obj);
		this->renderTree_->addObject(obj);
	}
}


void Disposer::newInline()
{
	this->nowSession_->newInline();
}

void Disposer::extendInline(Handler<RenderObject> obj)
{
	this->nowSession_->extendInline(obj);
}
void Disposer::extendBlock(BlockNode* blockNode)
{
	this->nowSession_->extendBlock(blockNode);
}


}}
