/**
 * Donut
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

#include <algorithm>
#include <donut/object/Heap.h>
#include <donut/object/Object.h>
#include <donut/object/Slot.h>

namespace donut {

struct Order {
	inline bool operator()(std::pair<unsigned int, Object*> const& a, std::pair<unsigned int, Object*> const& b)
	{
		return a.first < b.first;
	}
	inline bool operator()(std::pair<unsigned int, Object*> const& a, unsigned int const& b)
	{
		return a.first < b;
	}
	inline bool operator()(unsigned int const& a, std::pair<unsigned int, Object*> const& b)
	{
		return a < b.first;
	}
};

Slot::Slot()
:rev_()
,index_(-1)
{
}

Object* Slot::load() const
{
	if(this->index_ < 0){
		DONUT_EXCEPTION(Exception, "[BUG] No objects.");
	}
	std::pair<timestamp_t, Object*> const& p = this->rev_.at(this->index_);
	return p.second;
}

Object* Slot::store(Handler<Heap> const& heap, Object* obj)
{
	unsigned int now = heap->clock()->now();
	if( static_cast<int>(this->rev_.size())-1 != this->index_ ){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. this slot is not synchronized.");
	}
	if(this->index_ < 0 || ((std::pair<timestamp_t, Object*> const&)this->rev_.back()).first < now){
		this->rev_.push_back( std::pair<timestamp_t, Object*>(now, obj) );
		this->index_ = this->rev_.size()-1;
	}else{
		this->rev_.back() = std::pair<timestamp_t, Object*>(now, obj);
	}
	return obj;
}

bool Slot::have() const
{
	return this->index_ >= 0;
}

/**********************************************************************************
 * from clock
 **********************************************************************************/

XValue Slot::save()
{
	Handler<XObject> obj( new XObject );
	obj->set("index", this->index_);
	{
		Handler<XArray> list( new XArray );
		for( std::pair<timestamp_t, Object*> const& p : rev_ ) {
			Handler<XObject> sobj( new XObject );
			sobj->set("time", p.first);
			sobj->set("obj",p.second->toDescriptor());
			list->append(sobj);
		}
		obj->set("rev", list);
	}
	return obj;
}
Slot::Slot( Handler<Heap> const& heap, XValue const& data)
{
	Handler<XObject> obj( data.as<XObject>() );
	this->index_=obj->get<int>("index");
	for( XValue const& p : (*obj->get<XArray>("rev")) ) {
		Handler<XObject> sobj( p.as<XObject>() );
		this->rev_.push_back(
				std::pair<timestamp_t, Object*>(
						sobj->get<timestamp_t>("time"),
						heap->decodeDescriptor(sobj->get<object_desc_t>("obj")).get()
						));
	}
}
/**********************************************************************************
 * from clock
 **********************************************************************************/

void Slot::onBackNotify(Handler<Heap> const& heap)
{
	unsigned int const timestamp = heap->clock()->now();
	for(int i=this->index_; i>=0; --i){
		std::pair<timestamp_t, Object*> const& p = this->rev_[i];
		if(p.first <= timestamp){
			this->index_ = i;
			return;
		}
	}
	this->index_ = -1; //見つからないとここが呼ばれる
}
void Slot::onForwardNotify(Handler<Heap> const& heap)
{
	unsigned int const timestamp = heap->clock()->now();
	int const max = this->rev_.size();
	for(int i=this->index_+1; i<max; ++i){
		std::pair<timestamp_t, Object*> const& p = this->rev_[i];
		if(p.first > timestamp){
			this->index_ = i-1;
			return;
		}
	}
	this->index_ = max-1; //見つからないとここが呼ばれる
}

void Slot::onDiscardHistoryNotify( Handler<Heap> const& heap )
{
	if(this->rev_.size() <= 0){
		return;
	}
	std::vector<std::pair<timestamp_t, Object*> >::iterator end = this->rev_.begin()+index_;
	this->rev_.erase(this->rev_.begin(), end);
	this->index_ = 0;
}

void Slot::onDiscardFutureNotify( Handler<Heap> const& heap )
{
	if(this->rev_.size() <= 0){
		return;
	}
	this->rev_.erase(this->rev_.begin() + this->index_ + 1, this->rev_.end());
}
}
