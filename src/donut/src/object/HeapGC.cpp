/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/Heap.hpp>

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

	if( log().d() ){
		float const percent = (origObjectCount-newObjectCount)*100.0f/origObjectCount;
		this->log().d(TAG, "<Garbage collected> obj: %d -> %d, %0.2f%% collected",
				origObjectCount, newObjectCount, percent);
	}

	this->objectPoolMarked_.clear();
}

}
