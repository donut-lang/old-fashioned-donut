/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <cinamo/ClassUtil.h>
#include <donut/Donut.hpp>
#include <donut/object/ReactiveNativeObject.hpp>
#include "../World.hpp"
#include "../Heaven.hpp"
#include "../Angel.hpp"
#include "../heaven/ElementServant.hpp"
#include "../heaven/HaloServant.hpp"
#include "../heaven/ContentUnderlineServant.hpp"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/
struct ServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class ServantProvider;
class ServantObject : public ReactiveNativeObject {
private:
	Handler<Servant> servant_;
protected:
	ServantObject(ServantProvider* provider);
	virtual ~ServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Servant> const& target);
public:
	Handler<World> world() const;
	Handler<Servant> servant() const;
//public:
//	void onFutureDiscarded(Handler<Heap> const& heap);
//	void onHistoryDiscarded(Handler<Heap> const& heap);
//	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
//	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
//	XValue saveImpl( Handler<Heap> const& heap ) override final;
//	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class ServantProvider : public ReactiveProvider {
private:
	HandlerW<Heaven> heaven_;
protected:
	ServantProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~ServantProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/
template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
class ServantObjectBaseT : public ServantObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT)
protected:
	typedef ServantObjectBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	ServantObjectBaseT(ProviderT* provider);
	virtual ~ServantObjectBaseT() noexcept = default;
protected:
	void bootstrap(const Handler<Heap>& heap, const Handler<TargetT>& servant);
public:
	Handler<TargetT> servant() const;
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
};

template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
class ServantProviderBaseT : public ServantProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, ServantProvider)
protected:
	typedef ServantProviderBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	ServantProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~ServantProviderBaseT() noexcept = default;
public:
	template <typename... Args> inline
	Handler<ObjectT> newInstance(Args... args) {
		return HeapProvider::newInstance<ObjectT>( static_cast<ProviderT*>(this), args... );
	}
};

/**********************************************************************************************************************
 * ElementServant
 **********************************************************************************************************************/
struct ElementServantSideEffect : public ServantSideEffect{
	enum {
		None,
		DisableAnimation,
		EnableAnimation,
	} op;
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class ElementServantProvider;
class ElementServantObject : public ServantObjectBaseT<ElementServantProvider, ElementServantObject, ElementServant, ElementServantSideEffect> {
public:
	ElementServantObject(ElementServantProvider* provider);
	virtual ~ElementServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<ElementServant> const& servant);
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
};

class ElementServantProvider : public ServantProviderBaseT<ElementServantProvider, ElementServantObject, ElementServant, ElementServantSideEffect> {
public:
	ElementServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~ElementServantProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new ElementServantObject(this);
	}
};
/**********************************************************************************************************************
 * HaloServant
 **********************************************************************************************************************/
struct HaloServantSideEffect : public ServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class HaloServantProvider;
class HaloServantObject : public ServantObjectBaseT<HaloServantProvider, HaloServantObject, HaloServant, HaloServantSideEffect> {
public:
	HaloServantObject(HaloServantProvider* provider);
	virtual ~HaloServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<HaloServant> const& servant);
public:
	Handler<World> world() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
};
class HaloServantProvider : public ServantProviderBaseT<HaloServantProvider, HaloServantObject, HaloServant, HaloServantSideEffect> {
public:
	HaloServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~HaloServantProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new HaloServantObject(this);
	}
};
/**********************************************************************************************************************
 * ContentUnderlineServant
 **********************************************************************************************************************/
struct ContentUnderlineServantSideEffect : public ServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class ContentUnderlineServantProvider;
class ContentUnderlineServantObject : public ServantObjectBaseT<ContentUnderlineServantProvider, ContentUnderlineServantObject, ContentUnderlineServant, ContentUnderlineServantSideEffect> {
public:
	ContentUnderlineServantObject(ContentUnderlineServantProvider* provider);
	virtual ~ContentUnderlineServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<ContentUnderlineServant> const& servant);
public:
	Handler<World> world() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
};
class ContentUnderlineServantProvider : public ServantProviderBaseT<ContentUnderlineServantProvider, ContentUnderlineServantObject, ContentUnderlineServant, ContentUnderlineServantSideEffect> {
public:
	ContentUnderlineServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~ContentUnderlineServantProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new ContentUnderlineServantObject(this);
	}
};

}}
