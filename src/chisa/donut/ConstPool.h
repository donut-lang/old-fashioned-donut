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
#include "../Handler.h"
#include "object/Heap.h"
#include "source/Source.h"

namespace chisa {
namespace donut {

class ConstPool : public HandlerBody<ConstPool> {
private:
	Handler<Heap> heap_;
	Handler<Source> source_;
private:
	std::vector<Handler<FloatObject> > floatPool_;
	std::vector<Handler<StringObject> > stringPool_;
	std::vector<Handler<DonutClosureObject> > closPool_;
public:
	ConstPool(const Handler<Heap>& heap, const Handler<Source>& source);
	virtual ~ConstPool() noexcept = default;
	bool onFree() noexcept { return false; };
public:
	Handler<Object> getBool(const unsigned int& idx);
	Handler<Object> getInt(const unsigned int& idx);
	Handler<FloatObject> getFloat(const unsigned int& idx);
	Handler<StringObject> getString(const unsigned int& idx);
};

}}
