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
#include "../Decl.h"

#include "../../Handler.h"
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../util/VectorMap.h"

#include "../provider/Provider.h"
#include "../Clock.h"

#include "Object.h"
#include "DonutClosureObject.h"
#include "NativeClosureObject.h"

namespace chisa {
namespace donut {

class Heap final : public HandlerBody<Heap> {
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Clock> clock_;
	objectid_t objectId_;
	int sourceId_;
	int walkColor_;
	std::size_t gcLimit_;
	std::size_t gcLimitMax_;
private:
	util::VectorMap<std::string, Handler<HeapProvider> > providers_;
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

	Handler<DonutObject> boolProto_;
	Handler<DonutObject> intProto_;
	Handler<DonutObject> nullProto_;
public:
	Heap(logging::Logger& log, Handler<Clock> const& clock);
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
	Handler<ReactiveNativeClosureObject> createReactiveNativeClosureObject(std::string const& objectProviderName, std::string const& closureName, ReactiveNativeClosureObject::Signature f);
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
	util::XValue save();
	void load( util::XValue const& data);
public: /* 保存・復帰時にのみ使う */
	Handler<Object> decodeDescriptor( object_desc_t const& desc );
	Handler<HeapObject> decodeHeapDescriptor( object_desc_t const& desc );
	Handler<Source> decodeSourceID(int const& id);
public: /* Clockから呼ばれる */
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

}}
