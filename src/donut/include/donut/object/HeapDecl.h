/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/VectorMap.h>

#include "../Decl.h"
#include "../Clock.h"

#include "ObjectDecl.h"
#include "../provider/ProviderDecl.h"
#include "../provider/NativeClosureProviderDecl.h"

#include "DonutClosureObjectDecl.h"
#include "NativeClosureObjectDecl.h"

namespace donut {
using namespace cinamo;

class Heap final : public HandlerBody<Heap> {
	DEFINE_MEMBER_REF(public, Logger, log)
private:
	Handler<Clock> clock_;
	objectid_t objectId_;
	int sourceId_;
	int walkColor_;
	std::size_t gcLimit_;
	std::size_t gcLimitMax_;
private:
	VectorMap<std::string, Handler<HeapProvider> > providers_;
	std::vector<HeapObject*> objectPool_;
	std::vector<HeapObject*> objectPoolMarked_;
	std::vector<Source*> sourcePool_;
	Handler<DonutProvider> donutObjectProvider_;
	Handler<DonutClosureProvider> donutClosureObjectProvider_;
private:
	Handler<DonutObject> globalObject_;
	Handler<DonutObject> objectProto_;

	Handler<BoolProvider> boolProvider_;
	Handler<IntProvider> intProvider_;
	Handler<NullProvider> nullProvider_;

	Handler<FloatProvider> floatProvider_;
	Handler<StringProvider> stringProvider_;
	Handler<PureNativeClosureProvider> pureNativeClosureProvider_;
	Handler<ReactiveNativeClosureProvider> reactiveNativeClosureProvider_;
	Handler<HomuraProvider> homuraProvider_;

	Handler<DonutObject> boolProto_;
	Handler<DonutObject> intProto_;
	Handler<DonutObject> nullProto_;
public:
	Heap(Logger& log, Handler<Clock> const& clock);
	virtual ~Heap() noexcept = default;
	bool onFree() noexcept;
public:
	Handler<Clock> clock() const noexcept { return this->clock_; };
public:
	Handler<DonutProvider>& donutObjectProvider() { return donutObjectProvider_; };
	Handler<BoolProvider>& boolProvider() {return boolProvider_;};
	Handler<IntProvider>& intProvider() {return intProvider_;};
	Handler<NullProvider>& nullProvider() {return nullProvider_;};
public:
	Handler<DonutObject> objectProto() { return this->objectProto_; };
	Handler<Object> boolProto() {return boolProto_;};
	Handler<Object> intProto() {return intProto_;};
	Handler<Object> nullProto() {return nullProto_;};
public: /* オブジェクトを作ってそれをプールに登録し、メモリ管理の対象にする。 */
	Handler<DonutObject> createDonutObject();
	Handler<DonutObject> createEmptyDonutObject();
	Handler<DonutClosureObject> createDonutClosureObject( Handler<Source> const& src, unsigned int const& closureIndex, Handler<Object> const& scope );
	Handler<StringObject> createStringObject(std::string const& val);
	Handler<FloatObject> createFloatObject(float const& val);
	Handler<Object> createInt(int const& val);
	Handler<Object> createBool(bool const& val);
	Handler<Object> createNull();
	Handler<PureNativeClosureObject> createPureNativeClosureObject(std::string const& objectProviderName, std::string const& closureName, PureNativeClosureObject::Signature sig);
	template <typename __AntiSideEffect>
	Handler<ReactiveNativeClosureObject> createReactiveNativeClosureObject(std::string const& objectProviderName, std::string const& closureName, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature f);
public:
	Handler<Object> createObject( XValue const& xvalue );
	template <typename ObjectT, typename ProviderT, typename... Args>
	Handler<ObjectT> createObject( ProviderT* provider, Args... args ){
		Handler<ObjectT> obj ( new ObjectT(provider) );
		obj->bootstrap(args...);
		this->registerObject(obj);
		return obj;
	}
private:
	Handler<HomuraObject> createHomuraObject();
public: /* ヒープ管理 */
	void gc();
public: /* グローバルオブジェクトの管理 */
	bool hasGlobalObject( std::string const& name );
	void setGlobalObject( std::string const& name, Handler<Object> const& obj );
	Handler<Object> getGlobalObject( std::string const& name );
	Handler<DonutObject> global() { return this->globalObject_; }
public: /* プロバイダの管理 */
	void registerProvider( Handler<HeapProvider> const& provider );
	Handler<HeapProvider> findHeapProvider( std::string const& name ) const;
	Handler<Provider> findProvider( std::string const& name ) const;
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	XValue save();
	void load( XValue const& data);
public: /* 保存・復帰時にのみ使う */
	Handler<Object> decodeDescriptor( object_desc_t const& desc );
	Handler<HeapObject> decodeHeapDescriptor( object_desc_t const& desc );
	Handler<Source> decodeSourceID(int const& id);
public: /* Clockから呼ばれる */
	void onTickNotify();
	void onBackNotify();
	void onForwardNotify();
	void onDiscardFutureNotify();
	void onDiscardHistoryNotify();
public: /* Sourceから呼ばれる */
	Handler<Source> registerSource( Handler<Source> const& source );
	void unregisterSource( Source* source );
private:
	void initPrimitiveProviders();
	void initPrototypes();
	HeapObject* findHeapObjectFromID( objectid_t const& id );
	void registerObject( Handler<HeapObject> const& obj );
	int nextWalkColor();
};

}
