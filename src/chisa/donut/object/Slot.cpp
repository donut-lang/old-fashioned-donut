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

void Slot::onSeekNotify( const Handler<Heap>& heap )
{
	unsigned int const timestamp = heap->clock()->now();
	this->index_ = -1;
	for(int i=this->rev_.size()-1; i>=0; --i){
		std::pair<timestamp_t, Object*> const& p = this->rev_.at(i);
		if(p.first <= timestamp){
			this->index_ = i;
			return;
		}
	}
}

void Slot::onDiscardHistoryNotify( const Handler<Heap>& heap )
{
	if(this->index_ < 0 || this->rev_.size() <= 0){
		return;
	}
	std::vector<std::pair<timestamp_t, Object*> >::iterator end = this->rev_.begin()+index_;
	this->rev_.erase(this->rev_.begin(), end);
	this->index_ = this->rev_.size()-1;
}

void Slot::onDiscardFutureNotify( const Handler<Heap>& heap )
{
	if(this->index_ < 0 || this->rev_.size() <= 0){
		return;
	}
	this->rev_.erase(this->rev_.begin() + this->index_ + 1, this->rev_.end());
}

Object* Slot::load() const
{
	if(this->index_ < 0){
		throw DonutException(__FILE__, __LINE__, "[BUG] No objects.");
	}
	std::pair<timestamp_t, Object*> const& p = this->rev_.at(this->index_);
	return p.second;
}

Object* Slot::store(const Handler<Heap>& heap, Object* obj)
{
	unsigned int now = heap->clock()->now();
	if( static_cast<int>(this->rev_.size())-1 != this->index_ ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. this slot is not synchronized.");
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

}}
