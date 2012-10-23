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
#include "../../../src/chisa/gl/StringRenderer.h"
#include <math.h>

namespace chisa {
namespace gl {

TEST(StringRendererTest, BasicTest)
{
	StringRenderer r;
	geom::Area aa = r.measure("a");
	geom::Area ab = r.measure("あ");
	geom::Area area;
	std::size_t size;
	std::size_t max;
	std::tie(area, size, max) = r.calcMaximumStringLength("aあc", aa.width()+ab.width()+0.0001);
	ASSERT_EQ(3, max);
	ASSERT_EQ(2, size);
	ASSERT_FLOAT_EQ(aa.width()+ab.width(), area.width());

	std::tie(area, size, max) = r.calcMaximumStringLength("aあc", aa.width()+ab.width()-0.0001);
	ASSERT_EQ(3, max);
	ASSERT_EQ(1, size);
	ASSERT_FLOAT_EQ(aa.width(), area.width());
}

TEST(StringRendererTest, EmptySpaceTest)
{

	StringRenderer r;
	geom::Area area;
	std::size_t size;
	std::size_t max;
	std::tie(area, size, max) = r.calcMaximumStringLength("aあc", r.measure("a").width()-0.0001);
	ASSERT_EQ(3, max);
	ASSERT_EQ(0, size);
	ASSERT_FLOAT_EQ(0, area.width());
}

}}

