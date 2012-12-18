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
#include "Button.h"

namespace chisa {

namespace util {
namespace xml {
using namespace chisa::tk;

template <>
void parseAttr<TabCombo::ButtonPosition>(std::string const& name, TabCombo::ButtonPosition& v, TabCombo::ButtonPosition const& def, tinyxml2::XMLElement* elm)
{
	v=def;
	const char* val = elm->Attribute(name.c_str());
	if(val){
		std::string v(util::toLower(val));
		if(v == "top"){
			v=TabCombo::ButtonPosition::Top;
		}else if(v=="bottom"){
			v=TabCombo::ButtonPosition::Bottom;
		}else if(v=="right"){
			v=TabCombo::ButtonPosition::Right;
		}else if(v=="left"){
			v=TabCombo::ButtonPosition::Left;
		}
	}
}}}

namespace tk {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TabCombo, ElementGroup)
,buttonPosition_(ButtonPosition::Bottom)
{
	this->addAttribute("tab", this->buttonPosition_);
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
	this->addChild(h, "Button");
}
void TabCombo::addChild(std::size_t const& idx, Handler<Element> const& h)
{
	this->addChild(idx, h, "Button");
}

void TabCombo::addChild(Handler<Element> const& h, std::string const& text)
{
	Handler<Button> btn(new Button(log(), world(), top_));
	btn->text(text);
	this->buttonMap_.insert(h, btn);
	this->frame_->addChild(h);
	this->buttons_->addChild(btn);
}
void TabCombo::addChild(std::size_t const& idx, Handler<Element> const& h, std::string const& text)
{
	Handler<Button> btn(new Button(log(), world(), top_));
	btn->text(text);
	this->frame_->addChild(idx, h);
	this->buttons_->addChild(idx, btn);
}

Handler<Element> TabCombo::removeChild(std::size_t const& idx)
{
	Handler<Element> elm(this->frame_->removeChild(idx));
	auto it = this->buttonMap_.find(elm);
	Handler<Button> btn(it->second);
	this->buttonMap_.erase(it);
	this->buttons_->removeChild(btn);
	return elm;
}
Handler<Element> TabCombo::removeChild(Handler<Element> const& h)
{
	auto it = this->buttonMap_.find(h);
	Handler<Button> btn(it->second);
	this->buttonMap_.erase(it);
	this->buttons_->removeChild(btn);
	return 	this->frame_->removeChild(h);
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
	return this->frame_->bringChildToLast(e);
}
std::size_t TabCombo::bringChildToFront(Handler<Element> const& e)
{
	return this->frame_->bringChildToFront(e);
}
Handler<Element> TabCombo::findElementById(std::string const& id)
{
	return this->frame_->findElementById(id);
}
Handler<Element> TabCombo::findElementByPoint(geom::Vector const& screenPoint)
{
	return this->top_->findElementByPoint(screenPoint);
}
void TabCombo::idle(const float delta_ms)
{
	this->top_->idle(delta_ms);
}

std::string TabCombo::toString() const
{
	return util::format("(TabCombo %p with %d elements)", this, this->frame_->getChildCount());
}

void TabCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	this->top_->render(canvas, screenArea, area);
}

geom::Box TabCombo::measureImpl(geom::Box const& constraint)
{
	return this->top_->measure(constraint);
}

void TabCombo::layoutImpl(geom::Box const& size)
{
	this->top_->layout(size);
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
		this->top_->addChild(this->buttons_, SplitComboContext(0));
		break;
	case ButtonPosition::Left:
		this->top_->setMode(SplitCombo::SplitMode::Horizontal);
		this->buttons_->setMode(SplitCombo::SplitMode::Vertical);
		this->top_->addChild(this->buttons_, SplitComboContext(0));
		this->top_->addChild(this->frame_, SplitComboContext(1));
		break;
	case ButtonPosition::Bottom:
		this->top_->setMode(SplitCombo::SplitMode::Vertical);
		this->buttons_->setMode(SplitCombo::SplitMode::Horizontal);
		this->top_->addChild(this->frame_, SplitComboContext(1));
		this->top_->addChild(this->buttons_, SplitComboContext(0));
		break;
	case ButtonPosition::Top:
		this->top_->setMode(SplitCombo::SplitMode::Vertical);
		this->buttons_->setMode(SplitCombo::SplitMode::Horizontal);
		this->top_->addChild(this->buttons_, SplitComboContext(0));
		this->top_->addChild(this->frame_, SplitComboContext(1));
		break;
	}
	for( tinyxml2::XMLElement* e = element->FirstChildElement(); e; e=e->NextSiblingElement() ){
		const char* text = e->Attribute("label");
		if(text){
			this->addChild( factory->parseTree(this->self(), e), text);
		}else{
			this->addChild( factory->parseTree(this->self(), e));
		}
	}
}

}}
