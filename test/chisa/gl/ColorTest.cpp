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
#include "../../../src/chisa/gl/Color.h"

namespace chisa {
namespace gl {

#define TEST_GETTER_SETTER(color, name, val)\
	color.name(val);\
	ASSERT_FLOAT_EQ(val, color.name());

TEST(ColorTest, GetSetTest)
{
	Color c;
	TEST_GETTER_SETTER(c, red, 0.1);
	TEST_GETTER_SETTER(c, blue, 0.4);
	TEST_GETTER_SETTER(c, green, 0.21);
	TEST_GETTER_SETTER(c, alpha, 0.4);
}

TEST(ColorTest, AlphaMultiplyTest)
{
	Color c(1,1,1,1);
	Color const c1 = c.multAlpha(0.2f);
	ASSERT_FLOAT_EQ(0.2, c1.alpha());

	Color const c2 = c1.multAlpha(0.7f);
	ASSERT_FLOAT_EQ(0.2*0.7, c2.alpha());
}

}}

