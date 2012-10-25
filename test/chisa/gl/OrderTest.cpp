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

//前提条件
//http://www.sgi.com/tech/stl/StrictWeakOrdering.html

TEST(BufferOrderTest, IrreflexivityTest)
{
	internal::BufferOrder order;
	ASSERT_FALSE(order(std::make_pair(100,100),std::make_pair(100,100)));
}

TEST(BufferOrderTest, AntisymmetryTest)
{
	internal::BufferOrder order;
	ASSERT_NE(order(std::make_pair(1,2),std::make_pair(3,4)), order(std::make_pair(3,4),std::make_pair(1,2)));
}

TEST(BufferOrderTest, TransitivityTest)
{
	internal::BufferOrder order;
	ASSERT_EQ(order(std::make_pair(1,2),std::make_pair(3,4)) && order(std::make_pair(3,4),std::make_pair(5,6)), order(std::make_pair(1,2),std::make_pair(5,6)));
}

TEST(BufferOrderTest, ComplexTest)
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

TEST(SpriteOrderTest, IrreflexivityTest)
{
	internal::SpriteOrder order;
	ASSERT_FALSE(order(std::make_pair(100,100),std::make_pair(100,100)));
}

TEST(SpriteOrderTest, AntisymmetryTest)
{
	internal::SpriteOrder order;
	ASSERT_NE(order(std::make_pair(1,2),std::make_pair(3,4)), order(std::make_pair(3,4),std::make_pair(1,2)));
}

TEST(SpriteOrderTest, TransitivityTest)
{
	internal::SpriteOrder order;
	ASSERT_EQ(order(std::make_pair(1,2),std::make_pair(3,4)) && order(std::make_pair(3,4),std::make_pair(5,6)), order(std::make_pair(1,2),std::make_pair(5,6)));
}

TEST(SpriteOrderTest, ComplexTest)
{
	//どちらも大きいときだけtrue
	internal::SpriteOrder order;
	ASSERT_FALSE(order(std::make_pair(1,2),std::make_pair(1,3)));
	ASSERT_FALSE(order(std::make_pair(1,2),std::make_pair(1,1)));
	//横幅が小さい
	ASSERT_FALSE(order(std::make_pair(2,2),std::make_pair(1,3)));
	ASSERT_FALSE(order(std::make_pair(2,2),std::make_pair(1,1)));
	//横幅が大きい
	ASSERT_TRUE (order(std::make_pair(1,2),std::make_pair(2,3)));
	ASSERT_FALSE(order(std::make_pair(1,2),std::make_pair(2,1)));
}


}}

