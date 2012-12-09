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

#include "Heap.h"

namespace chisa {
namespace donut {

const static std::string TAG("HeapGC");

int Heap::nextWalkColor()
{
	return ++this->walkColor_;
}

void Heap::gc()
{

	// オブジェクトのお掃除
	this->objectPoolMarked_.clear();
	const int color = this->nextWalkColor();

	const std::size_t origObjectCount = this->objectPool_.size();
	this->objectPoolMarked_.reserve(origObjectCount);

	for(HeapObject*& obj : this->objectPool_){
		if(obj->used() ){
			obj->mark(self(), color);
		}
	}

	for(HeapObject*& obj : this->objectPool_){
		if(obj->color() != color){
			obj->erase();
		}else{
			this->objectPoolMarked_.push_back(obj);
		}
	}
	this->objectPool_.swap(this->objectPoolMarked_);
	const std::size_t newObjectCount = this->objectPool_.size();

	const std::size_t sourceCount = this->sourcePool_.size();
	//ソースのお掃除
	for( Source*& src : this->sourcePool_ ){
		if(!src->used()){
			src->erase();
		}
	}
	const std::size_t newSourceCount = this->sourcePool_.size();

	if( log().d() ){
		float const percent = (origObjectCount-newObjectCount)*100.0f/origObjectCount;
		float const spercent = (sourceCount-newSourceCount)*100.0f/sourceCount;
		this->log().d(TAG, "<Garbage collected> obj: %d -> %d, %0.2f%% collected; src: %d->%d, %0.2f%% collected",
				origObjectCount, newObjectCount, percent,
				sourceCount, newSourceCount, spercent
				);
	}

	this->objectPoolMarked_.clear();
}

}}
