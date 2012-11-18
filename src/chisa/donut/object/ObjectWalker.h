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

#include "Object.h"
namespace chisa {
namespace donut {
class Object;

class ObjectWalker {
private:
	int const color_;
public:
	ObjectWalker(const int color):color_(color){};
	virtual ~ObjectWalker() noexcept = default;
public:
	void start(HeapObject* obj) { obj->walk(this); };
	int color() const noexcept { return this->color_; };
public:
	virtual void onWalk(NativeObject* obj) = 0;
	virtual void onWalk(NativeClosureObject* obj) = 0;
	virtual void onWalk(DonutObject* obj) = 0;
};

}}
