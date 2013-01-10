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
#include <string>
#include <tarte/String.h>
#include <tarte/Dynamic.h>
#include <donut/Donut.h>
#include "ElementObject.h"
#include "../element/ElementGroup.h"

namespace chisa {
namespace tk {

template <typename DerivedProviderT, typename ObjectT, typename ElementT>
class ElementGroupProviderBaseT : public ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT> {
protected:
	typedef ElementGroupProviderBaseT<DerivedProviderT, ObjectT, ElementT> Super;
	ElementGroupProviderBaseT( Handler<Heap> const& heap, std::string const& name ):ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT>(heap, name){ this->registerClosures(); };
	ElementGroupProviderBaseT( Handler<Heap> const& heap ): ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT>(heap, ::tarte::demangle<ObjectT>() ){ this->registerClosures(); };
	virtual ~ElementGroupProviderBaseT() noexcept = default;
private:
	void registerClosures() {
		this->registerPureNativeClosure("getChildCount", &ObjectT::getChildCount);
		this->registerPureNativeClosure("getChildAt", &ObjectT::getChildAt);
		this->registerPureNativeClosure("frontChild", &ObjectT::frontChild);
		this->registerPureNativeClosure("lastChild", &ObjectT::lastChild);
	}
};


template <typename ProviderT, typename DerivedObjectT, typename ElementT>
class ElementGroupObjectBaseT : public ElementObjectBaseT<ProviderT, DerivedObjectT, ElementT>
{
protected:
	ElementGroupObjectBaseT(ProviderT* provider)
	:ElementObjectBaseT<ProviderT, DerivedObjectT, ElementT>(provider)
	{
	}
	virtual ~ElementGroupObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::tarte::format("(ElementGroupObject for \"%s\" %p)", ::tarte::demangle<DerivedObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<ElementT> const& element) {
		this->ElementObjectBaseT<ProviderT, DerivedObjectT, ElementT>::bootstrap(heap, element);
	}
public: /* 副作用なし */
	std::size_t getChildCount() const noexcept {
		return this->element()->getChildCount();
	}
	Handler<Element> getChildAt( std::size_t const& idx ) const noexcept {
		return this->element()->getChildAt(idx);
	}
	Handler<Element> lastChild() const noexcept {
		return this->element()->lastChild();
	}
	Handler<Element> frontChild() const noexcept {
		return this->element()->frontChild();
	}
public: /* TODO: 副作用あり */
	void addChild(Handler<Element> const& h) {
		this->element()->addChild(h);
	}
	void addChild(std::size_t const& idx, Handler<Element> const& h) {
		this->element()->addChild(idx, h);
	}
	Handler<Element> removeChild(std::size_t const& idx) {
		return this->element()->removeChild(idx);
	}
	Handler<Element> removeChild(Handler<Element> const& h) {
		return this->element()->removeChild(h);
	}
	std::size_t bringChildToLast(Handler<Element> const& e) {
		return this->element()->bringChildToLast(e);
	}
	std::size_t bringChildToFront(Handler<Element> const& e) {
		return this->element()->bringChildToFront(e);
	}
};

}}
