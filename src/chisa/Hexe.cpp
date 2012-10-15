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

#include "Hexe.h"

namespace chisa {

Hexe::Hexe(logging::Logger& log, const std::string& basepath)
:log_(log)
,basepath_(basepath)
{
}

Hexe::~Hexe()
{
}

std::string Hexe::toString() const
{
	return util::format("(Hexe %p)", this);
}

WorldGeist::WorldGeist(logging::Logger& log, std::weak_ptr<chisa::tk::World> world)
:log_(log)
,world_(world)
{

}
WorldGeist::~WorldGeist()
{

}

std::string WorldGeist::toString() const{
	return util::format("(WorldGeist %p)", this);
}


}
