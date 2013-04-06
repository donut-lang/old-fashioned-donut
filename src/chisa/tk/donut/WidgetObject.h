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
#include "../Widget.h"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

class World;
class Widget;
class WidgetObject;

// すべて共通のポインタとして使えるようにするためのクラス
class WidgetProvider : public ::donut::ReactiveProvider {
private:
	HandlerW<World> world_;
protected:
	WidgetProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world );
public:
	virtual ~WidgetProvider() noexcept = default;
public:
	Handler<World> world() const;
	virtual Handler<WidgetObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<Widget> const& widget) = 0;
};

class WidgetObject : public ReactiveNativeObject {
private:
	HandlerW<World> world_;
	Handler<Widget> const widget_;
protected:
	WidgetObject(WidgetProvider* provider);
	virtual ~WidgetObject() noexcept = default;
	inline Handler<World> world() const;
	void bootstrap(const Handler< ::donut::Heap>& heap, const Handler<Widget>& widget);
public:
	Handler<Widget> const& widget() const noexcept { return widget_; };
};

/**********************************************************************************************************************
 * 実装用テンプレートクラス
 **********************************************************************************************************************/

struct WidgetSideEffect {
	enum {
		None,
	} op;
	typedef WidgetSideEffect Super;
	template <typename Arc>
	void serialize(Arc& arc) {
	}
};

template <typename ProviderT, typename ObjectT, typename WidgetT, typename AntiT>
class WidgetProviderBaseT : public WidgetProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, WidgetProvider);
protected:
	typedef WidgetProviderBaseT<ProviderT, ObjectT, WidgetT, AntiT> Super;
	WidgetProviderBaseT( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world );
	WidgetProviderBaseT( Handler<Heap> const& heap, Handler<World> const& world );
	virtual ~WidgetProviderBaseT() noexcept = default;
private:
	void registerClosures();
private:
	virtual HeapObject* __internal__createInstanceForLoading() override final {
		return new ObjectT( static_cast<ProviderT*>(this) );
	}
	virtual Handler<WidgetObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<Widget> const& widget) override final
	{
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), heap, widget );
	}
};


template <typename ProviderT, typename ObjectT, typename WidgetT, typename AntiT>
class WidgetObjectBaseT : public WidgetObject
{
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT);
public:
	typedef WidgetObjectBaseT<ProviderT, ObjectT, WidgetT, AntiT> Super;
protected:
	inline ProviderT provider() const noexcept { return static_cast<ProviderT*>(this->WidgetObject::provider()); };
public:
	Handler<WidgetT> widget() const { return Handler<WidgetT>::__internal__fromRawPointerWithoutCheck(static_cast<WidgetT*>(WidgetObject::widget().get())); };
protected:
	WidgetObjectBaseT(ProviderT* provider);
	virtual ~WidgetObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::cinamo::format("(WidgetObject for \"%s\" %p)", ::cinamo::demangle<ObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<Widget> const& widget) {
		this->WidgetObject::bootstrap(heap, widget);
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

template<typename ProviderT, typename ObjectT, typename WidgetT, typename AntiT>
WidgetProviderBaseT<ProviderT, ObjectT, WidgetT, AntiT >::WidgetProviderBaseT(const Handler<Heap>& heap, const std::string& name, Handler<World> const& world)
:WidgetProvider(heap, name, world)
{
	registerClosures();
}

template<typename ProviderT, typename ObjectT, typename WidgetT, typename AntiT>
WidgetProviderBaseT<ProviderT, ObjectT, WidgetT, AntiT >::WidgetProviderBaseT(const Handler<Heap>& heap, Handler<World> const& world)
:WidgetProvider(heap, ::cinamo::demangle<ObjectT>(), world)
{
	registerClosures();
}

template<typename ProviderT, typename ObjectT, typename WidgetT, typename AntiT>
void WidgetProviderBaseT<ProviderT, ObjectT, WidgetT, AntiT >::registerClosures()
{
}


//-------------------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::WidgetObjectBaseT(ProviderT* provider)
:WidgetObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{
}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	WidgetSideEffect& side = newAnti;
	WidgetSideEffect const& old = val;
	switch(old.op) {
	case WidgetSideEffect::None:
		side.op = WidgetSideEffect::None;
		break;
	}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	WidgetSideEffect const& old = val;
	switch(old.op){
	case WidgetSideEffect::None:
		break;
	}
	return this->WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	WidgetSideEffect const& old = val;
	switch(old.op){
	case WidgetSideEffect::None:
		break;
	}
	return this->WidgetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

}}

