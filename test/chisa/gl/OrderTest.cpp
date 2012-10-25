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
#include "../../../src/chisa/gl/Canvas.h"
#include <math.h>

namespace chisa {
namespace gl {
#define ANTI(a,b,c,d) ASSERT_NE(order(std::make_pair(a,b),std::make_pair(c,d)), order(std::make_pair(c,d),std::make_pair(a,b)));
//前提条件
//http://www.sgi.com/tech/stl/StrictWeakOrdering.html

TEST(OrderTest, BasicTest)
{
	internal::BufferOrder order;
	//横幅が同じなら縦幅でソート
	ASSERT_TRUE (order(std::make_pair(1,2),std::make_pair(1,3)));
	ASSERT_FALSE(order(std::make_pair(1,2),std::make_pair(1,1)));
	//横幅が違うならそれだけで決まる
	ASSERT_FALSE(order(std::make_pair(2,2),std::make_pair(1,3)));
	ASSERT_FALSE(order(std::make_pair(2,2),std::make_pair(1,1)));
	//横幅が違うならそれだけで決まる
	ASSERT_TRUE (order(std::make_pair(1,2),std::make_pair(2,3)));
	ASSERT_TRUE (order(std::make_pair(1,2),std::make_pair(2,1)));
}

TEST(OrderTest, IrreflexivityTest)
{
	internal::BufferOrder order;
	ASSERT_FALSE(order(std::make_pair(100,100),std::make_pair(100,100)));
}

TEST(OrderTest, AntisymmetryTest)
{
	internal::BufferOrder order;
	ANTI(1,2,1,3);
	ANTI(1,2,1,1);
	ANTI(2,2,1,3);
	ANTI(2,2,1,1);
	ANTI(1,2,2,3);
	ANTI(1,2,2,1);
}

TEST(OrderTest, TransitivityTest)
{
	internal::BufferOrder order;
	ASSERT_EQ(order(std::make_pair(1,2),std::make_pair(3,4)) && order(std::make_pair(3,4),std::make_pair(5,6)), order(std::make_pair(1,2),std::make_pair(5,6)));
}

}}

