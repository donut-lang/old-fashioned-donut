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

namespace chisa {
namespace donut {

class World {
	DEFINE_MEMBER_REF(public, logging::Logger, log)
private:
	Handler<Code> code_;
private:
	unsigned int generation_;
private:
	BoolProvider boolProvider_;
	IntProvider intProvider_;
	NullProvider nullProvider_;
private:
	Handler<BaseObject> globalObject_;
	Handler<BaseObject> objectProto_;
	Handler<Object> boolProto_;
	Handler<Object> intProto_;
	Handler<Object> nullProto_;
	Handler<FloatObject> floatProto_;
	Handler<StringObject> stringProto_;
public:
	World(logging::Logger& log, Handler<Code> code);
	virtual ~World() noexcept = default;
public:
	unsigned int nextGeneration();

	BoolProvider& boolProvider() {return boolProvider_;};
	IntProvider& intProvider() {return intProvider_;};
	NullProvider& nullProvider() {return nullProvider_;};

	Handler<Object> boolProto() {return boolProto_;};
	Handler<Object> intProto() {return intProto_;};
	Handler<Object> nullProto() {return nullProto_;};

	Handler<Code> code() { return this->code_; }
	Handler<BaseObject> global() { return this->globalObject_; }
	Handler<BaseObject> objectProto() { return this->objectProto_; };
};

}}
