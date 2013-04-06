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
#include "../../Hexe.h"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

class World;
class GeistObject;

// すべて共通のポインタとして使えるようにするためのクラス
class GeistProvider : public ::donut::ReactiveProvider {
protected:
	GeistProvider( Handler<Heap> const& heap, std::string const& name );
public:
	virtual ~GeistProvider() noexcept = default;
};

class GeistObject : public ReactiveNativeObject {
private:
	HandlerW<WorldGeist> const geist_;
protected:
	GeistObject(GeistProvider* provider);
	virtual ~GeistObject() noexcept = default;
	inline Handler<World> world() const;
	inline void bootstrap(Handler< ::donut::Heap> const& heap, Handler<WorldGeist> const& geist){
		this->ReactiveNativeObject::bootstrap(heap);
		const_cast<HandlerW<WorldGeist>&>(geist_) = geist;
	}
public:
	Handler<WorldGeist> geist();
};

/**********************************************************************************************************************
 * 実装用テンプレートクラス
 **********************************************************************************************************************/

struct GeistSideEffect {
	enum {

	} op;
	typedef GeistSideEffect Super;
	template <typename Arc>
	void serialize(Arc& arc) {
	}
};

template <typename ProviderT, typename ObjectT, typename GeistT, typename AntiT>
class GeistProviderBaseT : public GeistProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, GeistProvider);
protected:
	typedef GeistProviderBaseT<ProviderT, ObjectT, GeistT, AntiT> Super;
	GeistProviderBaseT( Handler<Heap> const& heap, std::string const& name );
	GeistProviderBaseT( Handler<Heap> const& heap );
	virtual ~GeistProviderBaseT() noexcept = default;
private:
	virtual HeapObject* __internal__createInstanceForLoading() override final {
		return new ObjectT( static_cast<ProviderT*>(this) );
	}
public:
	template <typename... Args> inline
	Handler<ObjectT> newInstance(Args... args) {
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), args... );
	}
	virtual Handler<GeistObject> newInstance(Handler< ::donut::Heap> const& heap, Handler<WorldGeist> const& geist)
	{
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), heap, geist.cast<GeistT>() );
	}
};


template <typename ProviderT, typename ObjectT, typename GeistT, typename AntiT>
class GeistObjectBaseT : public GeistObject
{
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT);
public:
	typedef GeistObjectBaseT<ProviderT, ObjectT, GeistT, AntiT> Super;
protected:
	inline ProviderT provider() const noexcept { return static_cast<ProviderT*>(this->GeistObject::provider()); };
public:
	Handler<GeistT> geist() { return Handler<GeistT>::__internal__fromRawPointerWithoutCheck(static_cast<GeistT*>(GeistObject::geist().get())); };
protected:
	GeistObjectBaseT(ProviderT* provider);
	virtual ~GeistObjectBaseT() noexcept = default;
public:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override {
		return ::cinamo::format("(GeistObject for \"%s\" %p)", ::cinamo::demangle<ObjectT>().c_str());
	}
public:
	void bootstrap(Handler< ::donut::Heap> const& heap, Handler<GeistT> const& element) {
		this->GeistObject::bootstrap(heap, element);
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

template<typename ProviderT, typename ObjectT, typename GeistT, typename AntiT>
GeistProviderBaseT<ProviderT, ObjectT, GeistT, AntiT >::GeistProviderBaseT(const Handler<Heap>& heap, const std::string& name)
:GeistProvider(heap, name)
{
}

template<typename ProviderT, typename ObjectT, typename GeistT, typename AntiT>
GeistProviderBaseT<ProviderT, ObjectT, GeistT, AntiT >::GeistProviderBaseT(const Handler<Heap>& heap)
:GeistProvider(heap, ::cinamo::demangle<ObjectT>())
{
}

//-------------------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::GeistObjectBaseT(ProviderT* provider)
:GeistObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{

}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	//GeistSideEffect& side = newAnti;
	//GeistSideEffect const& old = val;
	//switch(old.op) {
	//}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->GeistObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(heap, val);
}

}}

