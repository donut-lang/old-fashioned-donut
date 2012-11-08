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
#include "World.h"
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

Slot::Slot(World* const pool, Object* const obj)
:world_(pool)
,rev_()
,index_()
{
	this->rev_.push_back( std::pair<gen_t, Object*>(0, nullptr) );
	this->rev_.push_back( std::pair<gen_t, Object*>(pool->nextGeneration(), obj) );
	this->index_ = this->rev_.begin()+1;
}

void Slot::seek(unsigned int gen)
{
	this->index_ = std::lower_bound(rev_.begin(), rev_.end(), gen, Order());
}

void Slot::discardHistory()
{
	std::size_t const idx = this->index_ - this->rev_.begin();
	this->rev_.erase(this->rev_.begin()+1, this->index_ );
	this->index_=this->rev_.begin()+idx;
}

void Slot::discardFuture()
{
	std::size_t const idx = this->index_ - this->rev_.begin();
	this->rev_.erase(this->index_+1, this->rev_.end());
	this->index_=this->rev_.begin()+idx;
}

Object* Slot::load()
{
	return this->index_->second;
}

gen_t Slot::firstGen() const noexcept
{
	return this->size() == 0 ? 0 : (this->rev_.begin()+1)->first;
}

gen_t Slot::lastGen() const noexcept
{
	return (this->rev_.end()-1)->first;
}

Object* Slot::store(Object* obj)
{
	this->discardFuture();
	this->rev_.push_back( std::pair<gen_t, Object*>(this->world_->nextGeneration(), obj) );
	return obj;
}

bool Slot::have() const
{
	return this->rev_.begin() != this->index_ && this->rev_.end() != this->index_;
}

}}
