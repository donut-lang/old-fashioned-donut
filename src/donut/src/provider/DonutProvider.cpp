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

#include <donut/object/Object.h>
#include <donut/object/Heap.h>
#include <donut/object/DonutClosureObject.h>
#include <donut/provider/DonutProvider.h>


namespace donut {


DonutProvider::DonutProvider( Handler<Heap> const& heap )
:Super(heap, "DonutObject")
{
	this->registerPureNativeClosure("has",[this](DonutObject* obj, std::string name){
		return obj->has(this->heap().lock(), name);
	});
	this->registerPureNativeClosure("hasOwn",[this](DonutObject* obj, std::string name){
		return obj->has(this->heap().lock(), name);
	});
}
DonutClosureProvider::DonutClosureProvider( Handler<Heap> const& heap )
:Super(heap,"DonutClosureObject")
{

}

}
