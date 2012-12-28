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

#include "Element.h"
#include "World.h"
#include "../gl/Canvas.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {

const std::string Element::AttrName::ForegroundColor("foreground-color");
const std::string Element::AttrName::BackgroundColor("background-color");

const std::string Element::AttrName::EdgeColor("edge-color");
const std::string Element::AttrName::EdgeWidth("edge-width");
const std::string Element::AttrName::Padding("padding");
const std::string Element::AttrName::Margin("margin");

const std::string Element::AttrName::Id("id");


Element::Element(Logger& log, HandlerW<World> world, HandlerW<Element> parent)
:log_(log)
,world_(world)
,parent_(parent)
,margin_(0)
,padding_(0)
,edgeColor_(gl::Black)
,edgeWidth_(0)
,foregroundColor_(gl::Black)
,backgroundColor_(gl::Color(0.9,0.9,0.9,1))
,relayoutRequested_(false)
,onFocused_(false)
{
	this->addAttribute(AttrName::Margin, this->margin_);
	this->addAttribute(AttrName::Padding, this->padding_);
	this->addAttribute(AttrName::EdgeWidth, this->edgeWidth_);
	this->addAttribute(AttrName::EdgeColor, this->edgeColor_);
	this->addAttribute(AttrName::ForegroundColor, this->foregroundColor_);
	this->addAttribute(AttrName::BackgroundColor, this->backgroundColor_);
	this->addAttribute(AttrName::Id, this->id_);
}

void Element::idle(const float delta_ms)
{
}

void Element::margin(geom::Space const& m)
{
	this->margin_ = m;
	this->requestRelayout();
}
void Element::padding(geom::Space const& p)
{
	this->padding_ = p;
	this->requestRelayout();
}
void Element::foregroundColor(gl::Color const& c)
{
	this->foregroundColor_ = c;
	this->notifyViewRefreshed();
}
void Element::backgroundColor(gl::Color const& c)
{
	this->backgroundColor_ = c;
	this->requestRelayout();
}

void Element::edgeColor(gl::Color const& c)
{
	this->edgeColor_ = c;
	this->requestRelayout();
}
void Element::edgeWidth(float const& f)
{
	this->edgeWidth_ = f;
	this->requestRelayout();
}

void Element::loadXml(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	for(auto it : this->attrMap_){
		//firstには要素名が、secondにはnodeをパースして設定する関数オブジェクトが入ってる
		it.second(element);
	}
	this->loadXmlImpl(factory, element);
}

Handler<Element> Element::findRootElement()
{
	Handler<Element> root(self());
	for(Handler<Element> element=parent().lock(); element; element=element->parent().lock()){
		root=element;
	}
	return root;
}

Handler<Element> Element::findElementById(std::string const& id)
{
	return id == this->id() ? this->self() : Handler<Element>();
}

Handler<Element> Element::findElementByPoint(geom::Vector const& screenPoint)
{
	return this->lastDrawnAreaInRoot().contain(screenPoint) ? this->self() : Handler<Element>();
}


std::string Element::toString() const
{
	return ::tarte::format("(Element %p)", this);
}

geom::Box Element::measure(geom::Box const& constraint)
{
	geom::Distance total(this->margin_.totalSpace()+this->padding_.totalSpace());
	return this->measureImpl(constraint-total)+total;
}

void Element::render(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	if( this->relayoutRequested_ ){
		this->forceRelayout();
	}

	//このエレメントの描かれる仮想的な位置(一部は隠れているかもしれない）
	this->lastPositionInRoot_ = ptInRoot;
	//このエレメントの描かれる、仮想的な位置のうちの実際の位置
	this->lastDrawnAreaInRoot_ = geom::Area(ptInRoot+mask.point(), mask.box());

	geom::Space const marginAndPdding( this->padding_+this->margin_ );

	// パディングとマージンを計算
	geom::Area const contentsAreaInRoot(ptInRoot, size_);
	geom::Area const marginedContentsAreaInRoot(this->margin_.apply(contentsAreaInRoot));
	geom::Area const paddedContentsAreaInRoot(this->padding_.apply(marginedContentsAreaInRoot));

	// 中身のエリアと、その衝突判定
	geom::Area const contentArea(marginAndPdding.apply(geom::Area(geom::ZERO, size_)));
	geom::Area const contentMask(contentArea.intersect(mask));

	// 中身のエリアの仮想的な位置と、実際に描画された中身
	this->lastInnerPositionInRoot_ = paddedContentsAreaInRoot.point();
	this->lastInnerDrawnAreaInRoot_ = geom::Area(ptInRoot+contentMask.point(), contentMask.box());

	//中身に掛かるマスクを計算するために、オフセットを取り除く
	geom::Area const innerMask(contentMask.point()-marginAndPdding.offset(), contentMask.box());

	{
		gl::Canvas::ScissorScope scissor(canvas, this->lastDrawnAreaInRoot());


		canvas.fillRect(this->backgroundColor_, marginedContentsAreaInRoot);
		canvas.drawRect(this->edgeWidth_, this->edgeColor_, paddedContentsAreaInRoot);
		this->renderImpl(canvas, paddedContentsAreaInRoot.point(), innerMask);
	}
}

void Element::layout(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	this->offsetFromParent_ = offsetFromParent;
	this->innerSize_ = size-(this->margin_.totalSpace()+this->padding_.totalSpace());
	this->innerOffsetFromParent_ = offsetFromParent+this->margin_.offset()+this->padding_.offset();
	this->size_ = size;
	this->layoutImpl(
			this->innerOffsetFromParent_,
			this->innerSize_);
}

bool Element::isValidationRoot() const noexcept
{
	return false;
}

void Element::requestRelayout()
{
	if(!this->relayoutRequested_){
		this->relayoutRequested_ = true;
		if(Handler<Element> p = this->parent().lock()) {
			if(!p->isValidationRoot()){
				p->requestRelayout();
			}
		}
	}
}

void Element::forceRelayout()
{
	this->measure(this->size());
	this->layout(this->offsetFromParent(), this->size());
	this->notifyRelayoutFinished();
}

void Element::notifyRelayoutFinished()
{
	this->relayoutRequested_= false;
}

void Element::notifyViewRefreshed()
{
	if( this->notifyViewRefreshedImpl() ) {
		this->requestRelayout();
	}
}

bool Element::notifyViewRefreshedImpl()
{
	return false;
}

void Element::onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen)
{
	this->onFocused_ = true;
}

void Element::onFocusLost(float const& timeMs)
{
	this->onFocused_ = false;
}


}}
