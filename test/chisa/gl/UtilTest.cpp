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

#include "../../TestCommon.hpp"
#include "../../../src/chisa/gl/Util.hpp"

namespace chisa {
namespace gl {

TEST(UtilTest, GetSetTest)
{
	ASSERT_EQ(16, getPower2Of(12));
	ASSERT_EQ(1, getPower2Of(0));
	ASSERT_EQ(128, getPower2Of(127));
	ASSERT_EQ(256, getPower2Of(129));
	ASSERT_EQ(1<<30, getPower2Of((1<<30)-1));
}

}}
