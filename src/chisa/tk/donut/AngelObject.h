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
#include <tarte/ClassUtil.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../World.h"
#include "../Heaven.h"
#include "../Angel.h"
#include "../heaven/LoneAngel.h"
#include "../heaven/TwinAngel.h"


namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Servant
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *
 **********************************************************************************************************************/

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
	typedef AngelObject Super;
	AngelObject(AngelProvider* provider);
	virtual ~AngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Angel> const& angel);
public:
	Handler<World> world() const;
	Handler<Angel> angel() const;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
//public:
//	void onFutureDiscarded(Handler<Heap> const& heap);
//	void onHistoryDiscarded(Handler<Heap> const& heap);
//	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
//	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
//	XValue saveImpl( Handler<Heap> const& heap ) override final;
//	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};


/**********************************************************************************************************************
 * LoneAngel
 **********************************************************************************************************************/
struct LoneAngelSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class LoneAngelProvider;
class LoneAngelObject : public AngelObject {
	INJECT_REACTIVE_OBJECT_ASPECT(LoneAngelSideEffect, LoneAngelObject);
public:
	LoneAngelObject(LoneAngelProvider* provider);
	virtual ~LoneAngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<LoneAngel> const& angel);
public:
	Handler<LoneAngel> angel() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class LoneAngelProvider : public AngelProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(LoneAngelSideEffect, LoneAngelProvider);
private:
	HandlerW<Heaven> heaven_;
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
struct TwinAngelSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class TwinAngelProvider;
class TwinAngelObject : public AngelObject {
	INJECT_REACTIVE_OBJECT_ASPECT(TwinAngelSideEffect, TwinAngelObject);
public:
	TwinAngelObject(TwinAngelProvider* provider);
	virtual ~TwinAngelObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<TwinAngel> const& angel);
public:
	Handler<TwinAngel> angel() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class TwinAngelProvider : public AngelProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(TwinAngelSideEffect, TwinAngelProvider);
private:
	HandlerW<Heaven> heaven_;
public:
	TwinAngelProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~TwinAngelProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new TwinAngelObject(this);
	}
};

}}
