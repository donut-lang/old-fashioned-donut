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
#include "ObjectWalker.h"

namespace chisa {
namespace donut {

const static std::string TAG("HeapGC");

void Heap::walkAndGC( ObjectWalker& walker )
{
	this->objectPoolMarked_.clear();
	const int color = walker.color();

	const std::size_t origObjectCount = this->objectPool_.size();

	for(HeapObject*& obj : this->objectPool_){
		if(obj->used() ){
			walker.start(obj);
		}
	}

	for(HeapObject*& obj : this->objectPool_){
		if(obj->walkColor() != color){
			obj->erase();
		}
	}
	this->objectPool_.swap(this->objectPoolMarked_);
	const std::size_t newObjectCount = this->objectPool_.size();

	if( log().d() ){
		this->log().d("TAG", "Garbage collected: %d -> %d, %0.2f%% collected.", origObjectCount, newObjectCount, ((float)(origObjectCount-newObjectCount))/origObjectCount );
	}

	this->objectPoolMarked_.clear();
}

void Heap::gc()
{
	class Collector : public ObjectWalker {
	private:
		std::vector<HeapObject*>& heap_;
	public:
		Collector(Heap* world, std::vector<HeapObject*>& heap)
		:ObjectWalker(world->nextWalkColor())
		,heap_(heap)
		{

		}
		virtual ~Collector() noexcept = default;
	public:
		virtual void onWalk(NativeObject* obj) override
		{
			heap_.push_back(obj);
		}
		virtual void onWalk(NativeClosureObject* obj) override
		{
			heap_.push_back(obj);
		}
		virtual void onWalk(DonutObject* obj) override
		{
			heap_.push_back(obj);
		}
	};
	Collector c(this, this->objectPoolMarked_);
	this->walkAndGC(c);
}

void Heap::seek(timestamp_t time) {
	class SeekWalker : public ObjectWalker {
	private:
		Heap* const world_;
		std::vector<HeapObject*>& heap_;
	public:
		SeekWalker(Heap* world, std::vector<HeapObject*>& heap)
		:ObjectWalker(world->nextWalkColor())
		,world_(world)
		,heap_(heap)
		{

		}
		virtual ~SeekWalker() noexcept = default;
	public:
		virtual void onWalk(NativeObject* obj) override
		{
			obj->seek(world_->time());
			heap_.push_back(obj);
		}
		virtual void onWalk(NativeClosureObject* obj) override
		{
			obj->seek(world_->time());
			heap_.push_back(obj);
		}
		virtual void onWalk(DonutObject* obj) override
		{
			obj->seek(world_->time());
			heap_.push_back(obj);
		}

	};
	SeekWalker w(this, this->objectPoolMarked_);
	this->walkAndGC(w);
}

}}
