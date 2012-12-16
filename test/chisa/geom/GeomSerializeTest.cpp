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

#include "../../TestCommon.h"
#include "../../../src/chisa/geom/Vector.h"
#include "../../../src/chisa/geom/Area.h"
#include "../../../src/chisa/util/XArchiver.h"
#include <math.h>

namespace chisa {
namespace geom {

TEST(GeomSerializeTest, VectorTest)
{
	util::XValue v;
	{
		Vector vec(0,2);
		util::XArchiverOut arc;
		arc << vec;
		arc >> v;
	}
	{
		Vector vec;
		util::XArchiverIn in(v);
		in >> vec;
		ASSERT_FLOAT_EQ(0, vec.x());
		ASSERT_FLOAT_EQ(2, vec.y());
	}
}

TEST(GeomSerializeTest, IntVectorTest)
{
	util::XValue v;
	{
		IntVector vec(0,2);
		util::XArchiverOut arc;
		arc << vec;
		arc >> v;
	}
	{
		IntVector vec;
		util::XArchiverIn in(v);
		in >> vec;
		ASSERT_EQ(0, vec.x());
		ASSERT_EQ(2, vec.y());
	}
}

TEST(GeomSerializeTest, AreaVectorTest)
{
	util::XValue v;
	{
		Area area(0,2,4,6);
		util::XArchiverOut arc;
		arc << area;
		arc >> v;
	}
	{
		Area area;
		util::XArchiverIn in(v);
		in >> area;
		ASSERT_FLOAT_EQ(0, area.x());
		ASSERT_FLOAT_EQ(2, area.y());
		ASSERT_FLOAT_EQ(4, area.width());
		ASSERT_FLOAT_EQ(6, area.height());
	}
}

TEST(GeomSerializeTest, SpaceVectorTest)
{
	util::XValue v;
	{
		Space area(0,2,4,6);
		util::XArchiverOut arc;
		arc << area;
		arc >> v;
	}
	{
		Space area;
		util::XArchiverIn in(v);
		in >> area;
		ASSERT_FLOAT_EQ(0, area.top());
		ASSERT_FLOAT_EQ(2, area.bottom());
		ASSERT_FLOAT_EQ(4, area.left());
		ASSERT_FLOAT_EQ(6, area.right());
	}
}

}}
