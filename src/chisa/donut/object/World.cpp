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

#include "World.h"

namespace chisa {
namespace donut {

World::World(logging::Logger& log, Handler<Code> code)
:log_(log)
,code_(code)
,generation_(0)
,boolProvider_(this)
,intProvider_(this)
,nullProvider_(this)
{

	this->globalObject_ = Handler<BaseObject>( new BaseObject(this) );
	this->objectProto_ = Handler<BaseObject>( new BaseObject(this) );

	this->intProto_ = this->intProvider().createPrototype( this->objectProto_ );
	this->boolProto_ = this->boolProvider().createPrototype( this->objectProto_ );
	this->nullProto_ = this->nullProvider().createPrototype( this->objectProto_ );
this->globalObject_->store(this, "Object", objectProto_);

	this->floatProto_ = Handler<FloatObject>( new FloatObject(this) );
	this->stringProto_ = Handler<StringObject>( new StringObject(this) );
	this->globalObject_->store(this, "Float", floatProto_);
	this->globalObject_->store(this, "String", stringProto_);
}

unsigned int World::nextGeneration()
{
	return ++this->generation_;
}

}}
