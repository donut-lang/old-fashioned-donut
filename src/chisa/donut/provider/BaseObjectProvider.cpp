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

#include <tinyxml2.h>
#include "Provider.h"
#include "../Exception.h"
#include "../object/World.h"
#include "../native/Bind.h"

namespace chisa {
namespace donut {
static const std::string TAG("BaseObjectProvider");


DonutObjectProvider::DonutObjectProvider(World* const world)
:ObjectProvider(world, "__native_base_object__")
{
}

tinyxml2::XMLElement* DonutObjectProvider::serialize(tinyxml2::XMLDocument* doc, Handler<Object> obj)
{
}

Handler<Object> DonutObjectProvider::deserialize(tinyxml2::XMLElement* xml)
{
}

}}
