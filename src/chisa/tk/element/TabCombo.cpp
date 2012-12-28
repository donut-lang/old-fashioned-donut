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

#include "TabCombo.h"
#include "ElementFactory.h"
#include "FrameCombo.h"
#include "SplitCombo.h"
#include "TabButton.h"

namespace tarte {
namespace xml {

using namespace chisa::tk;

template <>
void parseAttr<TabCombo::ButtonPosition>(std::string const& name, TabCombo::ButtonPosition& v, TabCombo::ButtonPosition const& def, tinyxml2::XMLElement* elm)
{
	v=def;
	const char* val = elm->Attribute(name.c_str());
	if(val){
		std::string vs(::tarte::toLower(val));
		if(vs == "top"){
			v=TabCombo::ButtonPosition::Top;
		}else if(vs=="bottom"){
			v=TabCombo::ButtonPosition::Bottom;
		}else if(vs=="right"){
			v=TabCombo::ButtonPosition::Right;
		}else if(vs=="left"){
			v=TabCombo::ButtonPosition::Left;
		}
	}
}

}}

namespace chisa {
namespace tk {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TabCombo, ElementGroup)
,buttonPosition_(ButtonPosition::Bottom)
{
	this->addAttribute("place", this->buttonPosition_);
}

TabCombo::~TabCombo() noexcept
{

}

std::size_t TabCombo::getChildCount() const noexcept
{
	return this->frame_->getChildCount();
}
Handler<Element> TabCombo::getChildAt(std::size_t const& idx) const noexcept
{
	return this->frame_->getChildAt(idx);
}
void TabCombo::addChild(Handler<Element> const& h)
{
	this->addChild(h, h->toString());
}
void TabCombo::addChild(std::size_t const& idx, Handler<Element> const& h)
{
	this->addChild(idx, h, h->toString());
}

void TabCombo::updateButtonState(Handler<Element> const& elm, Handler<TabButton> const& btn)
{
	Handler<TabButton> b = btn ? btn : (*this->buttonMap_.find(elm)).second;
	b->notifyCheckedStateChanged();
}

void TabCombo::addChild(Handler<Element> const& h, std::string const& title)
{
	//ボタンの作成
	Handler<TabButton> const btn(new TabButton(log(), world(), top_));
	btn->text(title);
	btn->setVertical( (this->buttonPosition_ == ButtonPosition::Left) || (this->buttonPosition_ == ButtonPosition::Right) );
	btn->tab(self().cast<TabCombo>());
	btn->element(h);

	//ボタンの登録
	Handler<Element> last = this->lastChild(); //もともとの最終エレメント
	this->buttonMap_.insert(h, btn);
	this->frame_->addChild(h);
	this->buttons_->addChild(btn, SplitComboContext(1));

	//トグル状態の変更を通知
	updateButtonState(h, btn);
	if(last){ //ボタンがない状態で最初の登録をした時、これはnullになる
		updateButtonState(last, Handler<TabButton>());
	}
}
void TabCombo::addChild(std::size_t const& idx, Handler<Element> const& h, std::string const& title)
{
	//ボタンの作成
	Handler<TabButton> const btn(new TabButton(log(), world(), top_));
	btn->text(title);
	btn->setVertical( (this->buttonPosition_ == ButtonPosition::Left) || (this->buttonPosition_ == ButtonPosition::Right) );
	btn->tab(self().cast<TabCombo>());
	btn->element(h);

	//ボタンの登録
	Handler<Element> last = this->lastChild(); //もともとの最終エレメント
	this->buttonMap_.insert(h, btn);
	this->frame_->addChild(idx, h);
	this->buttons_->addChild(idx, btn, SplitComboContext(1));

	//トグル状態の変更を通知
	updateButtonState(h, btn);
	if(last){ //ボタンがない状態で最初の登録をした時、これはnullになる
		updateButtonState(last, Handler<TabButton>());
	}
}

Handler<Element> TabCombo::removeChild(std::size_t const& idx)
{
	Handler<Element> elm(this->frame_->removeChild(idx));
	auto const it = this->buttonMap_.find(elm);
	Handler<TabButton> btn(it->second);
	this->buttonMap_.erase(it);
	this->buttons_->removeChild(btn);
	if(this->lastChild()){ //もうボタンが残っていない場合、nullが帰ってくるので
		updateButtonState(this->lastChild(), Handler<TabButton>());
	}
	return elm;
}
Handler<Element> TabCombo::removeChild(Handler<Element> const& h)
{
	auto it = this->buttonMap_.find(h);
	Handler<TabButton> btn(it->second);
	this->buttonMap_.erase(it);
	this->buttons_->removeChild(btn);
	this->frame_->removeChild(h);
	if(this->lastChild()){ //もうボタンが残っていない場合、nullが帰ってくるので
		updateButtonState(this->lastChild(), Handler<TabButton>());
	}
	return h;
}
Handler<Element> TabCombo::lastChild() const noexcept
{
	return this->frame_->lastChild();
}
Handler<Element> TabCombo::frontChild() const noexcept
{
	return this->frame_->frontChild();
}
std::size_t TabCombo::bringChildToLast(Handler<Element> const& e)
{
	Handler<Element> last(this->frame_->lastChild());
	if(last != e){
		std::size_t t = this->frame_->bringChildToLast(e);
		updateButtonState(e, Handler<TabButton>());
		updateButtonState(last, Handler<TabButton>());
		return t;
	}else{
		return this->frame_->getChildCount()-1;
	}
}
std::size_t TabCombo::bringChildToFront(Handler<Element> const& e)
{
	Handler<Element> front(this->frame_->frontChild());
	if(front != e) {
		std::size_t t = this->frame_->bringChildToFront(e);
		this->updateButtonState(e, Handler<TabButton>());
		this->updateButtonState(this->frame_->lastChild(), Handler<TabButton>());
		return t;
	}else{
		return this->frame_->getChildCount()-1;
	}
}
Handler<Element> TabCombo::findElementById(std::string const& id)
{
	return this->id() == id ? self() : this->frame_->findElementById(id);
}
Handler<Element> TabCombo::findElementByPoint(geom::Vector const& screenPoint)
{
	return this->top_->findElementByPoint(screenPoint);
}

void TabCombo::notifyRelayoutFinished()
{
	Element::notifyRelayoutFinished();
	this->top_->notifyRelayoutFinished();
}

void TabCombo::idle(const float delta_ms)
{
	this->top_->idle(delta_ms);
}

std::string TabCombo::toString() const
{
	return ::tarte::format("(TabCombo %p with %d elements)", this, this->frame_->getChildCount());
}

void TabCombo::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	this->top_->render(canvas, ptInRoot, mask);
}

geom::Box TabCombo::measureImpl(geom::Box const& constraint)
{
	return this->top_->measure(constraint);
}

void TabCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	this->top_->layout(offsetFromParent, size);
}

void TabCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	this->top_ = this->createChild<SplitCombo>();
	this->buttons_ = this->createChild<SplitCombo>();
	this->frame_ = this->createChild<FrameCombo>();
	switch(this->buttonPosition_) {
	case ButtonPosition::Right:
		this->top_->setMode(SplitCombo::SplitMode::Horizontal);
		this->buttons_->setMode(SplitCombo::SplitMode::Vertical);
		this->top_->addChild(this->frame_, SplitComboContext(1));
		this->top_->addChild(this->buttons_, SplitComboContext(1));
		break;
	case ButtonPosition::Left:
		this->top_->setMode(SplitCombo::SplitMode::Horizontal);
		this->buttons_->setMode(SplitCombo::SplitMode::Vertical);
		this->top_->addChild(this->buttons_, SplitComboContext(1));
		this->top_->addChild(this->frame_, SplitComboContext(1));
		break;
	case ButtonPosition::Bottom:
		this->top_->setMode(SplitCombo::SplitMode::Vertical);
		this->buttons_->setMode(SplitCombo::SplitMode::Horizontal);
		this->top_->addChild(this->frame_, SplitComboContext(1));
		this->top_->addChild(this->buttons_, SplitComboContext(1));
		break;
	case ButtonPosition::Top:
		this->top_->setMode(SplitCombo::SplitMode::Vertical);
		this->buttons_->setMode(SplitCombo::SplitMode::Horizontal);
		this->top_->addChild(this->buttons_, SplitComboContext(1));
		this->top_->addChild(this->frame_, SplitComboContext(1));
		break;
	}
	for( tinyxml2::XMLElement* e = element->FirstChildElement(); e; e=e->NextSiblingElement() ){
		const char* title = e->Attribute("title");
		if(title){
			this->addChild( factory->parseTree(this->self(), e), title);
		}else{
			this->addChild( factory->parseTree(this->self(), e));
		}
	}
}

}}
