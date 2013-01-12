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
#include <donut/object/ReactiveNativeObject.h>
#include "../Element.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

class World;
class Element;
class ElementObject;

// すべて共通のポインタとして使えるようにするためのクラス
class ElementProvider : public ::donut::ReactiveProvider {
private:
	HandlerW<World> world_;
protected:
	ElementProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world );
public:
	virtual ~ElementProvider() noexcept = default;
public:
	Handler<World> world() const;
	virtual Handler<ElementObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<Element> const& element) = 0;
};

class ElementObject : public ReactiveNativeObject {
private:
	HandlerW<World> world_;
	Handler<Element> const element_;
protected:
	ElementObject(ElementProvider* provider);
	virtual ~ElementObject() noexcept = default;
	inline Handler<World> world() const;
	inline void bootstrap(Handler< ::donut::Heap> const& heap, Handler<Element> const& element){
		this->ReactiveNativeObject::bootstrap(heap);
		const_cast<Handler<Element>&>(element_) = this->element_;
	}
public:
	Handler<Element> element() const { return element_; };
public:
	Handler<Element> findRootElement();
	Handler<Element> findElementById(std::string const& id);
};

/**********************************************************************************************************************
 * 実装用テンプレートクラス
 **********************************************************************************************************************/

template <typename DerivedProviderT, typename ObjectT, typename ElementT, typename __AntiSideEffectT>
class ElementProviderBaseT : public ElementProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(__AntiSideEffectT, DerivedProviderT);
protected:
	typedef ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT, __AntiSideEffectT> Super;
	ElementProviderBaseT( Handler<Heap> const& heap, std::string const& name );
	ElementProviderBaseT( Handler<Heap> const& heap );
	virtual ~ElementProviderBaseT() noexcept = default;
private:
	virtual HeapObject* __internal__createInstanceForLoading() override final {
		return new ObjectT( static_cast<DerivedProviderT*>(this) );
	}
	template <typename... Args> inline
	Handler<ObjectT> newInstance(Args... args) {
	}
	virtual Handler<ElementObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<Element> const& element) override final
	{
		return HeapProvider::newInstance<ObjectT>( static_cast<DerivedProviderT*>(this), heap, element.cast<ElementT>() );
	}
};


template <typename ProviderT, typename DerivedObjectT, typename ElementT, typename __AntiSideEffectT>
class ElementObjectBaseT : public ElementObject
{
	INJECT_REACTIVE_OBJECT_ASPECT(__AntiSideEffectT, DerivedObjectT);
public:
	typedef ElementObjectBaseT<ProviderT, DerivedObjectT, ElementT, __AntiSideEffectT> Super;
protected:
	inline ProviderT provider() const noexcept { return static_cast<ProviderT*>(this->ElementObject::provider()); };
	Handler<ElementT> element() const { return Handler<ElementT>::__internal__fromRawPointerWithoutCheck(static_cast<ElementT*>(ElementObject::element().get())); };
protected:
	ElementObjectBaseT(ProviderT* provider)
	:ElementObject(provider)
	{
	}
	virtual ~ElementObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::tarte::format("(ElementObject for \"%s\" %p)", ::tarte::demangle<DerivedObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<ElementT> const& element) {
		this->ElementObject::bootstrap(heap, element);
	}
};

}}

template<typename DerivedProviderT, typename ObjectT, typename ElementT, typename __AntiSideEffectT>
inline chisa::tk::ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT, __AntiSideEffectT >::ElementProviderBaseT(const Handler<Heap>& heap, const std::string& name)
:HeapProvider(heap, name)
{
}

template<typename DerivedProviderT, typename ObjectT, typename ElementT, typename __AntiSideEffectT>
inline chisa::tk::ElementProviderBaseT<DerivedProviderT, ObjectT, ElementT, __AntiSideEffectT >::ElementProviderBaseT(const Handler<Heap>& heap)
:HeapProvider(heap, ::tarte::demangle<ObjectT>())
{
}
