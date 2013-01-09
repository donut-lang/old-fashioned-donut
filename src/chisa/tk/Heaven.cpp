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

#include "Heaven.h"
#include "Angel.h"
#include "World.h"

namespace chisa {
namespace tk {

Heaven::Heaven(const Handler<World>& world)
:world_(world)
{
}

void Heaven::render(gl::Canvas& canvas)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->render(canvas);
	}
}

void Heaven::idle(const float delta_ms)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->idle(delta_ms);
	}
}

void Heaven::reshape(const geom::Area& area)
{
	//何もしなくて良いんじゃないかな？
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->reshape(area);
	}
}

}}
