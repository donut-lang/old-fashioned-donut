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
class World;
class Object;
class ObjectWalker;

class Slot {
	STACK_OBJECT(Slot);
private:
	World* world_;
	std::vector<std::pair<gen_t, Object*> > rev_;
	std::vector<std::pair<gen_t, Object*> >::iterator index_;
public:
	Slot(World* const world, Object* const obj);
	~Slot() noexcept = default;
public:
	Slot(const Slot& other):world_(other.world_),rev_(other.rev_),index_(other.index_){}
	Slot(Slot&& other):world_(other.world_), index_(other.index_){ other.rev_.swap(rev_); }
	Slot& operator=(const Slot& other) { world_=other.world_;rev_=other.rev_;index_=other.index_;return *this; }
	Slot& operator=(Slot&& other) { world_=other.world_;rev_.swap(other.rev_);index_=other.index_;return *this; }
public:
	void seek( gen_t gen );
	void discardHistory();
	void discardFuture();
	Object* load() const;
	Object* store( Object* obj );
	gen_t firstGen() const noexcept;
	gen_t lastGen() const noexcept;
	bool have() const;
	inline std::size_t size() const noexcept { return this->rev_.size()-1; };
	void walk(ObjectWalker* walker);
};

}}
