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
#include "Decl.h"
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include <vector>

namespace chisa {
namespace donut {
class Heap;
class Object;

class Slot {
	STACK_OBJECT(Slot);
private:
	std::vector<std::pair<timestamp_t, Object*> > rev_;
	int index_;
public:
	Slot();
	~Slot() noexcept = default;
public:
	Slot(const Slot& other):rev_(other.rev_),index_(other.index_){}
	Slot(Slot&& other):index_(other.index_){ other.rev_.swap(rev_); }
	Slot& operator=(const Slot& other) { rev_=other.rev_;index_=other.index_;return *this; }
	Slot& operator=(Slot&& other) { rev_.swap(other.rev_);index_=other.index_;return *this; }
private:
	timestamp_t firstGen() const noexcept;
	timestamp_t lastGen() const noexcept;
public:
	void mark(const Handler<Heap>& heap, int color);
	void onDiscardHistoryNotify( const Handler<Heap>& heap );
	void onDiscardFutureNotify( const Handler<Heap>& heap );
	void onSeekNotify( const Handler<Heap>& heap );
	Object* load() const;
	Object* store( const Handler<Heap>& heap, Object* obj );
	bool have() const;
};

}}
