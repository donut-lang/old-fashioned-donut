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
#include <cinamo/ClassUtil.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../World.h"
#include "../Heaven.h"
#include "../Angel.h"
#include "../heaven/LoneAngel.h"
#include "../heaven/TwinAngel.h"


namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

struct AngelSideEffect{
	enum {
		AttatchTarget,
		DetatchTarget
	} op;
	Handler<AngelTarget> attatchedTarget_;
	Handler<AngelTarget> detatchedTarget_;
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class AngelProvider : public ReactiveProvider {
private:
	HandlerW<Heaven> heaven_;
protected:
	AngelProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~AngelProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

class AngelObject : public ReactiveNativeObject {
private:
	Handler<Angel> angel_;
protected:
	AngelObject(AngelProvider* provider);
	virtual ~AngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Angel> const& angel);
public:
	Handler<World> world() const;
	Handler<Angel> angel() const;
};

/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
class AngelProviderBaseT : public AngelProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, AngelProvider);
protected:
	typedef AngelProviderBaseT<ProviderT, ObjectT, AngelT, AntiT> Super;
	AngelProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~AngelProviderBaseT() noexcept = default;
public:
	template <typename... Args> inline
	Handler<ObjectT> newInstance(Args... args) {
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), args... );
	}
};

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
class AngelObjectBaseT : public AngelObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT);
protected:
	typedef AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT> Super;
	AngelObjectBaseT(ProviderT* provider);
	virtual ~AngelObjectBaseT() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Angel> const& angel);
public:
	Handler<AngelT> angel() const;
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
};


/**********************************************************************************************************************
 * LoneAngel
 **********************************************************************************************************************/
struct LoneAngelSideEffect : public AngelSideEffect {
	template <typename Arc>
	void serialize(Arc& arc) {
		AngelSideEffect::serialize(arc);
	}
};

class LoneAngelProvider;
class LoneAngelObject : public AngelObjectBaseT <LoneAngelProvider, LoneAngelObject, LoneAngel, LoneAngelSideEffect>{
public:
	LoneAngelObject(LoneAngelProvider* provider);
	virtual ~LoneAngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<LoneAngel> const& angel);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(const Handler<Heap>& heap) const override final;
};

class LoneAngelProvider : public AngelProviderBaseT <LoneAngelProvider, LoneAngelObject, LoneAngel, LoneAngelSideEffect>{
public:
	LoneAngelProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~LoneAngelProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new LoneAngelObject(this);
	}
};


/**********************************************************************************************************************
 * TwinAngel
 **********************************************************************************************************************/
struct TwinAngelSideEffect : public AngelSideEffect {
	template <typename Arc>
	void serialize(Arc& arc) {
		AngelSideEffect::serialize(arc);
	}
};

class TwinAngelProvider;
class TwinAngelObject : public AngelObjectBaseT<TwinAngelProvider, TwinAngelObject, TwinAngel, TwinAngelSideEffect> {
public:
	TwinAngelObject(TwinAngelProvider* provider);
	virtual ~TwinAngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<TwinAngel> const& angel);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(const Handler<Heap>& heap) const override final;
};

class TwinAngelProvider : public AngelProviderBaseT<TwinAngelProvider, TwinAngelObject, TwinAngel, TwinAngelSideEffect> {
public:
	TwinAngelProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~TwinAngelProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new TwinAngelObject(this);
	}
};

}}
