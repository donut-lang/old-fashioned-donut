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
#include "../heaven/HaloServant.h"
#include "../heaven/ElementServant.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

class ServantObject;
class ServantProvider : public ReactiveProvider {
private:
	HandlerW<Heaven> heaven_;
public:
	ServantProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~ServantProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

class ServantObject : public ReactiveNativeObject {
private:
	Handler<Servant> servant_;
protected:
	typedef ServantObject Super;
	ServantObject(ServantProvider* provider);
	virtual ~ServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Servant> const& target);
public:
	Handler<World> world() const;
	Handler<Servant> servant() const;
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
 * HaloServant
 **********************************************************************************************************************/
struct HaloServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class HaloServantProvider;
class HaloServantObject : public ServantObject {
	INJECT_REACTIVE_OBJECT_ASPECT(HaloServantSideEffect, HaloServantObject);
public:
	HaloServantObject(ServantProvider* provider);
	virtual ~HaloServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<HaloServant> const& servant);
public:
	Handler<World> world() const;
	Handler<HaloServant> servant() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};
class HaloServantProvider : public ServantProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(HaloServantSideEffect, HaloServantProvider);
private:
	HandlerW<Heaven> heaven_;
public:
	HaloServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~HaloServantProvider() noexcept = default;
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new HaloServantObject(this);
	}
};

/**********************************************************************************************************************
 * ElementServant
 **********************************************************************************************************************/
struct ElementServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class ElementServantProvider;
class ElementServantObject : public ServantObject {
	INJECT_REACTIVE_OBJECT_ASPECT(ElementServantSideEffect, ElementServantObject);
public:
	ElementServantObject(ServantProvider* provider);
	virtual ~ElementServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<ElementServant> const& servant);
public:
	Handler<World> world() const;
	Handler<ElementServant> servant() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};
class ElementServantProvider : public ServantProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(ElementServantSideEffect, ElementServantProvider);
private:
	HandlerW<Heaven> heaven_;
public:
	ElementServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~ElementServantProvider() noexcept = default;
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new ElementServantObject(this);
	}
};

}}
