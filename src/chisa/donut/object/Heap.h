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
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../util/VectorMap.h"
#include "../../Handler.h"

#include "../source/Source.h"

#include "../provider/Provider.h"
#include "../Clock.h"

#include "Object.h"
#include "DonutObject.h"
#include "NativeObject.h"

namespace chisa {
namespace donut {

class Heap : public HandlerBody<Heap> {
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Clock> clock_;
	objectid_t objectId_;
	int walkColor_;
	std::size_t gcLimit_;
	std::size_t gcLimitMax_;
private:
	util::VectorMap<std::string, Handler<HeapObjectProvider> > providers_;
	std::vector<HeapObject*> objectPool_;
	std::vector<HeapObject*> objectPoolMarked_;
	std::vector<Source*> sourcePool_;
	Handler<DonutObjectProvider> donutObjectProvider_;
	Handler<DonutClosureObjectProvider> donutClosureObjectProvider_;
private:
	Handler<DonutObject> globalObject_;
	Handler<DonutObject> objectProto_;

	Handler<BoolProvider> boolProvider_;
	Handler<IntProvider> intProvider_;
	Handler<NullProvider> nullProvider_;

	Handler<FloatProvider> floatProvider_;
	Handler<StringProvider> stringProvider_;
	Handler<PureNativeObjectProvider> pureNativeClosureProvider_;

	Handler<DonutObject> boolProto_;
	Handler<DonutObject> intProto_;
	Handler<DonutObject> nullProto_;
public:
	Heap(logging::Logger& log, const Handler<Clock>& clock);
	virtual ~Heap() noexcept = default;
	bool onFree() noexcept;
public:
	Handler<Clock> clock() const noexcept { return this->clock_; };
public:
	Handler<DonutObjectProvider>& donutObjectProvider() { return donutObjectProvider_; };
	Handler<BoolProvider>& boolProvider() {return boolProvider_;};
	Handler<IntProvider>& intProvider() {return intProvider_;};
	Handler<NullProvider>& nullProvider() {return nullProvider_;};

	Handler<DonutObject> objectProto() { return this->objectProto_; };
	Handler<Object> boolProto() {return boolProto_;};
	Handler<Object> intProto() {return intProto_;};
	Handler<Object> nullProto() {return nullProto_;};

	Handler<HeapObjectProvider> getHeapProvider( const std::string& name ) const;
	Handler<Provider> getProvider( const std::string& name ) const;
	Handler<DonutObject> global() { return this->globalObject_; }
private:
	void registerObject( const Handler<HeapObject>& obj );
	int nextWalkColor();
public: /* オブジェクトを作ってそれをプールに登録し、メモリ管理の対象にする。 */
	Handler<DonutObject> createDonutObject();
	Handler<DonutObject> createEmptyDonutObject();
	Handler<DonutClosureObject> createDonutClosureObject( const Handler<Source>& src, unsigned int const& closureIndex, const Handler<Object>& scope );
	Handler<StringObject> createStringObject(const std::string& val);
	Handler<FloatObject> createFloatObject(const float& val);
	Handler<Object> createInt(const int& val);
	Handler<Object> createBool(const bool& val);
	Handler<Object> createNull();
	Handler<PureNativeClosureObject> createPureNativeClosureObject(const std::string& objectProviderName, const std::string& closureName, PureNativeClosureObject::Signature sig);
public: /* ヒープ管理 */
	void gc();
	Handler<Object> decodeDescriptor( object_desc_t const& desc );
	Handler<HeapObject> decodeHeapDescriptor( object_desc_t const& desc );
private: //内部使用
	HeapObject* findHeapObjectFromID( objectid_t const& id );
public: /* 外部との接続 */
	void registerProvider( Handler<HeapObjectProvider> const& provider );
	void registerGlobalObject( std::string const& name, Handler<Object> const& obj );
	bool hasGlobalObject( std::string const& name );
	Handler<Object> loadGlobalObject( std::string const& name );
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	util::XValue save();
	void load( util::XValue const& data);
private:
	void initPrimitiveProviders();
	void initPrototypes();
public: /* Clockから呼ばれる */
	void onDiscardFutureNotify();
	void onDiscardHistoryNotify();
	void onSeekNotify();
};

}}
