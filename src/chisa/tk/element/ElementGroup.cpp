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

#include "ElementGroup.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(ElementGroup)
{

}

ElementGroup::~ElementGroup() noexcept
{

}

void ElementGroup::idle(const float delta_ms)
{
}

/*
void ElementGroup::loadXml(element::ElementGroupFactory* const factory, tinyxml2::XMLElementGroup* const element)
{
	for(auto it : this->attrMap_){
		//firstには要素名が、secondにはnodeをパースして設定する関数オブジェクトが入ってる
		it.second(element);
	}
	this->loadXmlImpl(factory, element);
}

Handler<ElementGroup> ElementGroup::findRootElement()
{
	Handler<ElementGroup> root(self());
	for(Handler<ElementGroup> element=parent().lock(); element; element=element->parent().lock()){
		root=element;
	}
	return root;
}
*/

Handler<Element> ElementGroup::removeChild(std::size_t const& idx)
{
	if(idx >= child_.size()) {
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Invalid index: %d >= %d", idx, child_.size());
	}
	auto it = this->child_.begin()+idx;
	Handler<Element> element (*(it));
	this->child_.erase(it);
	return element;
}
Handler<Element> ElementGroup::removeChild(Handler<Element> const& h)
{
	auto it = std::find(this->child_.begin(), this->child_.end(), h);
	if(it == child_.begin()) {
		throw logging::Exception(__FILE__, __LINE__, "[BUG] %s does not have %s.", this->toString().c_str(), h->toString().c_str());
	}
	this->child_.erase(it);
	return h;
}

Handler<Element> ElementGroup::lastChild() const noexcept
{
	return this->child_.size() <= 0 ? Handler<Element>() : this->child_.back();
}

Handler<Element> ElementGroup::findElementById(std::string const& id)
{
	return id == this->id() ? this->self() : Handler<Element>();
}

Handler<Element> ElementGroup::findElementByPoint(geom::Vector const& screenPoint)
{
	if(!this->screenArea().contain(screenPoint)){
		return Handler<ElementGroup>();
	}
	for(Handler<Element>& child : this->child_) {
		if(child->screenArea().contain(screenPoint)){
			return child->findElementByPoint(screenPoint);
		}
	}
	return this->self();
}

std::size_t ElementGroup::bringChildToLast(Handler<Element> const& e)
{
	auto it = std::find(child_.begin(), child_.end(), e);
	std::size_t s = std::distance(child_.begin(), it);
	if(it == this->child_.end()){
		throw logging::Exception(__FILE__, __LINE__, "Element: %s is not contained in this combo.", e->toString().c_str());
	}
	child_.erase(it);
	child_.push_back(e);
	this->layout(this->screenArea().box());
	return s;
}
std::size_t ElementGroup::bringChildToFront(Handler<Element> const& e)
{
	auto it = std::find(child_.begin(), child_.end(), e);
	std::size_t s = std::distance(child_.begin(), it);
	if(it == this->child_.end()){
		throw logging::Exception(__FILE__, __LINE__, "Element: %s is not contained in this combo.", e->toString().c_str());
	}
	child_.erase(it);
	child_.insert(this->child_.begin(), e);
	this->layout(this->screenArea().box());
	return s;
}

std::string ElementGroup::toString() const
{
	return util::format("(ElementGroup %p)", this);
}

}}}
