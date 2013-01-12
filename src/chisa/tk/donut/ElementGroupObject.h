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

/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/

struct ElementGroupSideEffect : public ElementSideEffect {
	enum {
		None=0,
	} op;
	ElementGroupSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		ElementSideEffect::serialize(arc);
	}
};

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ElementGroupProviderBaseT : public ElementProviderBaseT<ProviderT, ObjectT, ElementT,AntiT> {
protected:
	typedef ElementGroupProviderBaseT<ProviderT, ObjectT, ElementT,AntiT> Super;
	ElementGroupProviderBaseT( Handler<Heap> const& heap, std::string const& name );
	ElementGroupProviderBaseT( Handler<Heap> const& heap );
	virtual ~ElementGroupProviderBaseT() noexcept = default;
private:
	void registerClosures();
};


template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ElementGroupObjectBaseT : public ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>
{
protected:
	typedef typename chisa::tk::ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::ResultType ResultType;
	typedef typename chisa::tk::ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::AntiSideEffect AntiSideEffect;
	ElementGroupObjectBaseT(ProviderT* provider);
	virtual ~ElementGroupObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::tarte::format("(ElementGroupObject for \"%s\" %p)", ::tarte::demangle<ObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<ElementT> const& element) {
		this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::bootstrap(heap, element);
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
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
};

/**********************************************************************************************************************
 * テンプレートクラスの実装
 **********************************************************************************************************************/

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
ElementGroupProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::ElementGroupProviderBaseT(const Handler<Heap>& heap, const std::string& name)
:HeapProvider(heap, name)
{
	this->registerClosures();
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
ElementGroupProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::ElementGroupProviderBaseT(const Handler<Heap>& heap)
:HeapProvider(heap, ::tarte::demangle<ObjectT>())
{
	this->registerClosures();
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
void ElementGroupProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::registerClosures() {
	this->registerPureNativeClosure("getChildCount", &ObjectT::getChildCount);
	this->registerPureNativeClosure("getChildAt", &ObjectT::getChildAt);
	this->registerPureNativeClosure("frontChild", &ObjectT::frontChild);
	this->registerPureNativeClosure("lastChild", &ObjectT::lastChild);
}

//-------------------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::ElementGroupObjectBaseT(ProviderT* provider)
:ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>(provider)
{
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
void ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{
	this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onFutureDiscarded(heap);
}
template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
void ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{
	this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onHistoryDiscarded(heap);
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
inline typename ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::ResultType
ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	ElementGroupSideEffect& side = newAnti;
	ElementGroupSideEffect const& old = val;
	switch(old.op){
	}
	return std::make_tuple(true, newAnti);
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
inline typename ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::ResultType
ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ElementGroupSideEffect& side = val;
	if(side.op == ElementGroupSideEffect::None){
		return this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onBack(heap, val);
	}
	return this->execAntiSideEffect(heap, val);
}

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
inline typename ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::ResultType
ElementGroupObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ElementGroupSideEffect& side = val;
	if(side.op == ElementGroupSideEffect::None){
		return this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::onForward(heap, val);
	}
	return this->execAntiSideEffect(heap, val);
}

}}
