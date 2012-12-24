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

#include <donut/provider/DonutProvider.h>
#include <donut/object/Object.h>
#include <donut/object/DonutClosureObject.h>

namespace donut {


DonutProvider::DonutProvider( Handler<Heap> const& heap )
:HeapProviderImpl<DonutObject>(heap, "DonutObject")
{

}
DonutClosureProvider::DonutClosureProvider( Handler<Heap> const& heap )
:HeapProviderImpl<DonutClosureObject>(heap,"DonutClosureObject")
{

}

}
