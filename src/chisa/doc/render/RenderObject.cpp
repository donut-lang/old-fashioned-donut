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

#include "RenderObject.h"
#include "RenderTree.h"
#include "../../gl/DrawableManager.h"

namespace chisa {
namespace doc {

RenderObject::RenderObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth)
:parentTree_(parentTree)
,parentNode_(parentNode)
,dirty_(true)
,area_()
,relDepth_(relDepth)
{
}

void RenderObject::render(gl::Canvas& canvas, const float depth)
{
	this->drawable()->draw(canvas, this->area_.point(), geom::Area(geom::ZERO, this->area_.box()), depth+this->relDepth_);
}
void RenderObject::onHidden()
{
	if(!this->drawable_){
		return;
	}
	if(Handler<RenderTree> tree = this->parentTree_.lock()){
		this->drawablew_=this->drawable_;
		tree->registerDrawable(this->drawable_);
		this->drawable_.reset();
	}else{
		this->drawable_.reset();
		this->drawablew_.reset();
	}
}

Handler<gl::Drawable> RenderObject::drawable()
{
	if(!dirty_){
		if(this->drawable_){
			return this->drawable_;
		}else if(!this->drawablew_.expired()){
			this->drawable_ = this->drawablew_.lock();
			this->drawablew_.reset();
			return this->drawable_;
		}
	}
	this->drawablew_.reset();
	this->drawable_ = this->realize();
	this->dirty_ = false;
	this->cachedSize_ = this->drawable_->size();
	return this->drawable_;
}

HandlerW<RenderTree> RenderObject::parentTree()
{
	return this->parentTree_;
}
Node* RenderObject::parentNode()
{
	return this->parentNode_;
}

geom::Box RenderObject::size()
{
	if(!this->dirty_){
		return this->cachedSize_;
	}else{
		return this->drawable()->size();
	}
}
float RenderObject::width()
{
	return this->size().width();
}
float RenderObject::height()
{
	return this->size().height();
}

//-----------------------------------------------------------------------------
NormalDrawableObject::NormalDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, std::string const& repl, geom::Box const& size)
:RenderObject(parentTree, parentNode, relDepth),repl_(repl),size_(size)
{

}
Handler<gl::Drawable> NormalDrawableObject::realize()
{
	if(Handler<RenderTree> tree = this->parentTree().lock()){
		return tree->drawableManager()->queryDrawable(this->repl_, this->size_);
	}else{
		return Handler<gl::Drawable>();
	}
}

//-----------------------------------------------------------------------------

TextDrawableObject::TextDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, std::string const& str, const float size, Handler<gl::Font> font, gl::TextDrawable::Style style, gl::TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor)
:RenderObject(parentTree, parentNode, relDepth)
,str_(str)
,size_(size)
,font_(font)
,style_(style)
,deco_(deco)
,color_(color)
,backColor_(backColor)
{

}
Handler<gl::Drawable> TextDrawableObject::realize()
{
	if(Handler<RenderTree> tree = this->parentTree().lock()){
		return tree->drawableManager()->queryText(str_,size_,font_,style_,deco_,color_,backColor_);
	}else{
		return Handler<gl::Drawable>();
	}
}

}}
