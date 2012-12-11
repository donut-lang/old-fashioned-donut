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
#include "../../util/XVal.h"
#include <vector>

namespace chisa {
namespace donut {
class Heap;
class Object;

class Slot final {
	STACK_OBJECT(Slot);
private:
	std::vector<std::pair<timestamp_t, Object*> > rev_;
	int index_;
public:
	Slot();
	~Slot() noexcept = default;
public:
	Slot(Slot const& other):rev_(other.rev_),index_(other.index_){}
	Slot(Slot&& other):index_(other.index_){ other.rev_.swap(rev_); }
	Slot& operator=(Slot const& other) { rev_=other.rev_;index_=other.index_;return *this; }
	Slot& operator=(Slot&& other) { rev_.swap(other.rev_);index_=other.index_;return *this; }
private:
	timestamp_t firstGen() const noexcept;
	timestamp_t lastGen() const noexcept;
public:
	void mark(Handler<Heap> const& heap, int color);
	Object* load() const;
	Object* store( Handler<Heap> const& heap, Object* obj );
	bool have() const;
public: /* 処理系の保存・復帰をします。 */
	util::XValue save();
	Slot( Handler<Heap> const& heap, util::XValue const& data );
public: /* クロックから呼ばれる */
	void onDiscardHistoryNotify( Handler<Heap> const& heap );
	void onDiscardFutureNotify( Handler<Heap> const& heap );
	void onSeekNotify( Handler<Heap> const& heap );
};

}}
