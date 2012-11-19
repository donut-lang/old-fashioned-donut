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

#include "Slot.h"
#include "Heap.h"
#include "Object.h"
#include <algorithm>

namespace chisa {
namespace donut {

struct Order {
	bool operator()(std::pair<unsigned int, Object*>const&a, std::pair<unsigned int, Object*>const&b)
	{
		return a.first < b.first;
	}
	bool operator()(std::pair<unsigned int, Object*>const&a, unsigned int b)
	{
		return a.first < b;
	}
	bool operator()(unsigned int a, std::pair<unsigned int, Object*>const&b)
	{
		return a < b.first;
	}
};

Slot::Slot()
:rev_()
,index_(-1)
{
}

void Slot::seek( const Handler<Heap>& heap, timestamp_t timestamp )
{
	this->index_ = std::lower_bound(rev_.begin(), rev_.end(), timestamp, Order()) - this->rev_.begin();
}

void Slot::discardHistory()
{
	if(this->index_ < 0){
		return;
	}
	if( this->rev_.size() > 0 ) {
		std::vector<std::pair<timestamp_t, Object*> >::iterator end = this->rev_.begin()+index_;
		this->rev_.erase(this->rev_.begin(), end);
		this->index_ = this->rev_.size()-1;
	}
}

void Slot::discardFuture()
{
	if(this->index_ < 0){
		return;
	}
	if( this->rev_.size() > 0 ) {
		this->rev_.erase(this->rev_.begin() + this->index_ + 1, this->rev_.end());
	}
}

Object* Slot::load() const
{
	if(this->index_ < 0){
		throw DonutException(__FILE__, __LINE__, "[BUG] No objects.");
	}
	return this->rev_[this->index_].second;
}

timestamp_t Slot::firstGen() const noexcept
{
	if(this->rev_.size() <= 0){
		return 0;
	}
	return this->rev_.at(0).first;
}

timestamp_t Slot::lastGen() const noexcept
{
	if(this->rev_.size() <= 0){
		return 0;
	}
	return (this->rev_.end()-1)->first;
}

Object* Slot::store(const Handler<Heap>& heap, Object* obj)
{
	this->discardFuture();
	unsigned int now = heap->clock()->now();
	if(lastGen() < now){
		this->rev_.push_back( std::pair<timestamp_t, Object*>(now, obj) );
		this->index_ = this->rev_.size()-1;
	}else{
		std::vector<std::pair<timestamp_t, Object*> >::iterator last = this->rev_.end()-1;
		*last = std::pair<timestamp_t, Object*>(now, obj);
	}
	return obj;
}

bool Slot::have() const
{
	return this->index_ >= 0;
}

}}
