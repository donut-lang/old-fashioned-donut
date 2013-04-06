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
#include "ElementObject.h"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

class World;
class Element;
class ButtonObject;

/**********************************************************************************************************************
 * 実装用テンプレートクラス
 **********************************************************************************************************************/

struct ButtonSideEffect {
	enum {
		None
	} op;
	typedef ElementSideEffect Super;
	template <typename Arc>
	void serialize(Arc& arc) {
	}
	ButtonSideEffect():op(None){

	}
};

template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ButtonProviderBaseT : public ElementProviderBaseT<ProviderT, ObjectT, ElementT, AntiT > {
protected:
	typedef ButtonProviderBaseT<ProviderT, ObjectT, ElementT, AntiT> Super;
	ButtonProviderBaseT( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world );
	ButtonProviderBaseT( Handler<Heap> const& heap, Handler<World> const& world );
	virtual ~ButtonProviderBaseT() noexcept = default;
};


template <typename ProviderT, typename ObjectT, typename ElementT, typename AntiT>
class ButtonObjectBaseT : public ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>
{
public:
	typedef ButtonObjectBaseT<ProviderT, ObjectT, ElementT, AntiT> Super;
protected:
	ButtonObjectBaseT(ProviderT* provider);
	virtual ~ButtonObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::cinamo::format("(ButtonObject for \"%s\" %p)", ::cinamo::demangle<ObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<ElementT> const& element) {
		this->ElementObjectBaseT<ProviderT, ObjectT, ElementT, AntiT>::bootstrap(heap, element);
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

template<typename ProviderT, typename ObjectT, typename ButtonT, typename AntiT>
ButtonProviderBaseT<ProviderT, ObjectT, ButtonT, AntiT >::ButtonProviderBaseT(const Handler<Heap>& heap, const std::string& name, Handler<World> const& world)
:ElementProviderBaseT<ProviderT, ObjectT, ButtonT, AntiT >(heap, name, world)
{
}

template<typename ProviderT, typename ObjectT, typename ButtonT, typename AntiT>
ButtonProviderBaseT<ProviderT, ObjectT, ButtonT, AntiT >::ButtonProviderBaseT(const Handler<Heap>& heap, Handler<World> const& world)
:ElementProviderBaseT<ProviderT, ObjectT, ButtonT, AntiT >(heap, ::cinamo::demangle<ObjectT>(), world)
{
}

//-------------------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ButtonObjectBaseT(ProviderT* provider)
:ElementObjectBaseT<ProviderT, ObjectT, ButtonT, AntiT>(provider)
{
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{

}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	ButtonSideEffect& side = newAnti;
	ButtonSideEffect const& old = val;
	switch(old.op) {
	case ButtonSideEffect::None:
		break;
	}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ButtonSideEffect const& old = val;
	if( old.op == ButtonSideEffect::None ){
		return ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(heap, val);
	}
	return this->ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	ButtonSideEffect const& old = val;
	if( old.op == ButtonSideEffect::None ){
		return ElementObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(heap, val);
	}
	return this->ButtonObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

}}

