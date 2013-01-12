/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "DecoUtil.h"
#include "Canvas.h"
#include "../geom/Vector.h"

namespace chisa {
namespace gl {

void renderCubicLine(gl::Canvas& cv, float width, gl::Color const& color, geom::Point const& pt1, geom::Point const& pt2)
{
	geom::Distance delta( pt1-pt2 );
	geom::Point const vec_( delta.y()/delta.x() , -1 );
	geom::Point const vec( vec_ / (vec_.length()/(delta.length()/7)) );

	geom::Point last (pt1);
	for(int x=10; x >= 0; --x) {
		geom::Point next( ((pt1 * x + pt2*(10-x)) / 10)+vec*std::sin(3.14*x/10) );
		cv.drawLine(width, color, last, next );
		last=next;
	}
}

}}
