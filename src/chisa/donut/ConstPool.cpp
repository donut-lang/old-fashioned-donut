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

#include "ConstPool.h"

namespace chisa {
namespace donut {

ConstPool::ConstPool(const Handler<Heap>& heap, const Handler<Source>& source)
:heap_(heap)
,source_(source)
{
}

Handler<Object> ConstPool::getBool(const unsigned int& idx)
{
	return heap_->createBool( source_->getBool( idx ) );
}

Handler<Object> ConstPool::getInt(const unsigned int& idx)
{
	return heap_->createInt( source_->getInt( idx ) );
}

Handler<FloatObject> ConstPool::getFloat(const unsigned int& idx)
{
	if(idx >= floatPool_.size()){
		floatPool_.insert(floatPool_.end(), 1+idx-floatPool_.size(), Handler<FloatObject>());
	}
	Handler<FloatObject> obj = floatPool_[idx];
	if( !obj ){
		obj = floatPool_[idx] = heap_->createFloatObject( source_->getFloat(idx) );
	}
	return obj;
}

Handler<StringObject> ConstPool::getString(const unsigned int& idx)
{
	if(idx >= stringPool_.size()){
		stringPool_.insert(stringPool_.end(), 1+idx-stringPool_.size(), Handler<StringObject>());
	}
	Handler<StringObject> obj = stringPool_[idx];
	if( !obj ){
		obj = stringPool_[idx] = heap_->createStringObject( source_->getString(idx) );
	}
	return obj;
}

}}
