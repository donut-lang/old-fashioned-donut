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
#include "../../Handler.h"
#include "../code/Code.h"
#include "Object.h"
#include "../provider/PrimitiveProvider.h"
#include "../provider/BaseObjectProvider.h"

namespace chisa {
namespace donut {

class World {
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Code> code_;
private:
	unsigned int generation_;
private:
	std::map<std::string, Handler<Provider> > providers_;
	Handler<DonutObjectProvider> donutObjectProvider_;
	Handler<BoolProvider> boolProvider_;
	Handler<IntProvider> intProvider_;
	Handler<NullProvider> nullProvider_;
private:
	Handler<DonutObject> globalObject_;
	Handler<DonutObject> objectProto_;
	Handler<DonutObject> boolProto_;
	Handler<DonutObject> intProto_;
	Handler<DonutObject> nullProto_;
public:
	World(logging::Logger& log, Handler<Code> code);
	virtual ~World() noexcept = default;
public:
	unsigned int nextGeneration();
	Handler<DonutObjectProvider>& donutObjectProvider() { return donutObjectProvider_; };
	Handler<BoolProvider>& boolProvider() {return boolProvider_;};
	Handler<IntProvider>& intProvider() {return intProvider_;};
	Handler<NullProvider>& nullProvider() {return nullProvider_;};

	Handler<DonutObject> objectProto() { return this->objectProto_; };
	Handler<DonutObject> boolProto() {return boolProto_;};
	Handler<DonutObject> intProto() {return intProto_;};
	Handler<DonutObject> nullProto() {return nullProto_;};
private:
	Handler<Code> code() { return this->code_; }
	Handler<DonutObject> global() { return this->globalObject_; }
public:
	Handler<DonutObject> createDonutObject();
	Handler<DonutObject> createEmptyDonutObject();
	Handler<Object> createInt(const int& val);
	Handler<Object> createBool(const bool& val);
	Handler<Object> createNull();
public:
	void registerProvider( Handler<Provider> provider );
};

}}
