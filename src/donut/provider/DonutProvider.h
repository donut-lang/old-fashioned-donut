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

#pragma once
#include "Provider.h"

namespace donut {

class DonutObject;
class DonutClosureObject;
class DonutProvider final : public HeapProviderImpl<DonutObject> {
public:
	DonutProvider( Handler<Heap> const& heap );
	virtual ~DonutProvider() noexcept = default;
};
class DonutClosureProvider final : public HeapProviderImpl<DonutClosureObject> {
public:
	DonutClosureProvider( Handler<Heap> const& heap );
	virtual ~DonutClosureProvider() noexcept = default;
};

}
