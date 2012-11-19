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
#include <set>
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../Handler.h"
#include "../code/Code.h"
#include "Object.h"
#include "DonutObject.h"
#include "NativeObject.h"
#include "../provider/Provider.h"
#include "../Clock.h"

namespace chisa {
namespace donut {

class Heap : public HandlerBody<Heap> {
public:
	bool onFree() noexcept;
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Clock> clock_;
	uintptr_t objectId_;
	int walkColor_;
	std::size_t gcLimit_;
private:
	std::map<std::string, Handler<Provider> > providers_;
	std::vector<HeapObject*> objectPool_;
	std::vector<HeapObject*> objectPoolMarked_;
	Handler<DonutObjectProvider> donutObjectProvider_;
private:
	Handler<DonutObject> globalObject_;
	Handler<DonutObject> objectProto_;

	Handler<BoolProvider> boolProvider_;
	Handler<IntProvider> intProvider_;
	Handler<NullProvider> nullProvider_;
	Handler<DonutObject> boolProto_;
	Handler<DonutObject> intProto_;
	Handler<DonutObject> nullProto_;
public:
	Heap(logging::Logger& log, const Handler<Clock>& clock);
	virtual ~Heap() noexcept = default;
	Handler<Clock> clock() const noexcept { return this->clock_; };
	void bootstrap();
	tinyxml2::XMLElement* serialize(tinyxml2::XMLDocument* doc);
	void deserialize(tinyxml2::XMLElement* xml);
public:
	Handler<DonutObjectProvider>& donutObjectProvider() { return donutObjectProvider_; };
	Handler<BoolProvider>& boolProvider() {return boolProvider_;};
	Handler<IntProvider>& intProvider() {return intProvider_;};
	Handler<NullProvider>& nullProvider() {return nullProvider_;};

	Handler<DonutObject> objectProto() { return this->objectProto_; };
	Handler<Object> boolProto() {return boolProto_;};
	Handler<Object> intProto() {return intProto_;};
	Handler<Object> nullProto() {return nullProto_;};

	Handler<Provider> getProvider( const std::string& name ) const;
	Handler<DonutObject> global() { return this->globalObject_; }
private:
	void registerObject( Handler<HeapObject> obj );
	int nextWalkColor();
	void seek(timestamp_t time);
public:
	Handler<DonutObject> createDonutObject();
	Handler<DonutObject> createEmptyDonutObject();
	Handler<DonutClosureObject> createDonutClosureObject( Handler<Closure> closure, Handler<Object> scope );
	Handler<StringObject> createStringObject(const std::string& val);
	Handler<FloatObject> createFloatObject(const float& val);
	Handler<Object> createInt(const int& val);
	Handler<Object> createBool(const bool& val);
	Handler<Object> createNull();
	Handler<PureNativeClosureObject> createPureNativeClosureObject(const std::string& objectProviderName, const std::string& closureName, PureNativeClosureEntry::Signature sig);
public:
	void registerProvider( Handler<Provider> provider );
	void gc();
};

}}
