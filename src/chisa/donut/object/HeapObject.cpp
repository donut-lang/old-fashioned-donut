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

#include "Object.h"
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "../object/World.h"
namespace chisa {
namespace donut {

HeapObject::HeapObject(World* const world, const std::string& providerName)
:world_(world)
,providerName_(providerName)
,id_(0)
,erased_(0)
{
}

}}
