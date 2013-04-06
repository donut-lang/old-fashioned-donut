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
#include <cinamo/String.h>
#include <cinamo/Dynamic.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../Element.h"

namespace chisa {
using namespace cinamo;

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
		const_cast<Handler<Element>&>(element_) = element;
	}
public:
	Handler<Element> element() const { return element_; };
public:
	Handler<Element> findRootElement();
	Handler<Object> findElementById(std::string const& id);
	bool isEnabled();
};

/**********************************************************************************************************************
 * 実装用テンプレートクラス
 **********************************************************************************************************************/

struct ElementSideEffect {
	enum {
		None,
		SetEnabledOrDisabled,
		SetEdgeColor
	} op;
	bool enabled;
	gl::Color color;
	typedef ElementSideEffect Super;
	template <typename Arc>
	void serialize(Arc& arc) {
	}
};

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ElementProviderBaseT : public ElementProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, ElementProvider);
protected:
	typedef ElementProviderBaseT<ProviderT, ObjectT, ElementT, AntiT> Super;
	ElementProviderBaseT( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world );
	ElementProviderBaseT( Handler<Heap> const& heap, Handler<World> const& world );
	virtual ~ElementProviderBaseT() noexcept = default;
private:
	void registerClosures();
private:
	virtual HeapObject* __internal__createInstanceForLoading() override final {
		return new ObjectT( static_cast<ProviderT*>(this) );
	}
	virtual Handler<ElementObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<Element> const& element) override final
	{
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), heap, element.cast<ElementT>() );
	}
};


template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ElementObjectBaseT : public ElementObject
{
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT);
public:
	typedef ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT> Super;
protected:
	inline ProviderT provider() const noexcept { return static_cast<ProviderT*>(this->ElementObject::provider()); };
public:
	Handler<ElementT> element() const { return Handler<ElementT>::__internal__fromRawPointerWithoutCheck(static_cast<ElementT*>(ElementObject::element().get())); };
protected:
	ElementObjectBaseT(ProviderT* provider);
	virtual ~ElementObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::cinamo::format("(ElementObject for \"%s\" %p)", ::cinamo::demangle<ObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<ElementT> const& element) {
		this->ElementObject::bootstrap(heap, element);
	}
public:
	std::tuple<std::nullptr_t,bool,AntiT> setEnabled( bool newstate );
	std::tuple<std::nullptr_t,bool,AntiT> setEdgeColor( std::string const& color );
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

template<typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
ElementProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::ElementProviderBaseT(const Handler<Heap>& heap, const std::string& name, Handler<World> const& world)
:ElementProvider(heap, name, world)
{
	registerClosures();
}

template<typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
ElementProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::ElementProviderBaseT(const Handler<Heap>& heap, Handler<World> const& world)
:ElementProvider(heap, ::cinamo::demangle<ObjectT>(), world)
{
	registerClosures();
}

template<typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
void ElementProviderBaseT<ProviderT, ObjectT, ElementT, AntiT >::registerClosures()
{
	this->registerReactiveNativeClosure("setEnabled", &ObjectT::setEnabled);
	this->registerReactiveNativeClosure("setEdgeColor", &ObjectT::setEdgeColor);
}


//-------------------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ElementObjectBaseT(ProviderT* provider)
:ElementObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{
}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	ElementSideEffect& side = newAnti;
	ElementSideEffect const& old = val;
	switch(old.op) {
	case ElementSideEffect::SetEnabledOrDisabled:
		side.op = ElementSideEffect::SetEnabledOrDisabled;
		side.enabled = element()->enabled();
		element()->enabled( old.enabled );
		break;
	case ElementSideEffect::SetEdgeColor:
		side.op = ElementSideEffect::None;
		side.op = ElementSideEffect::SetEdgeColor;
		side.color = element()->edgeColor();
		element()->edgeColor( old.color );
		break;
	case ElementSideEffect::None:
		side.op = ElementSideEffect::None;
		break;
	}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ElementSideEffect const& old = val;
	switch(old.op){
	case ElementSideEffect::SetEdgeColor:
	case ElementSideEffect::SetEnabledOrDisabled:
	case ElementSideEffect::None:
		break;
	}
	return this->ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ElementSideEffect const& old = val;
	switch(old.op){
	case ElementSideEffect::SetEdgeColor:
	case ElementSideEffect::SetEnabledOrDisabled:
	case ElementSideEffect::None:
		break;
	}
	return this->ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
std::tuple<std::nullptr_t,bool,AntiT> ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::setEnabled(bool newstate)
{
		bool const oldstate = this->element()->enabled();
		AntiT anti;
		ElementSideEffect& side = anti;
		side.op=ElementSideEffect::SetEnabledOrDisabled;
		side.enabled = oldstate;
		element()->enabled(newstate);
		return std::tuple<std::nullptr_t, bool, AntiT>(nullptr, true, anti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
std::tuple<std::nullptr_t,bool,AntiT> ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::setEdgeColor( std::string const& color )
{
		AntiT anti;
		ElementSideEffect& side = anti;
		side.op=ElementSideEffect::SetEdgeColor;
		side.color = element()->edgeColor();
		element()->edgeColor( chisa::gl::Color::fromString(color) );
		return std::tuple<std::nullptr_t, bool, AntiT>(nullptr, true, anti);
}

}}

