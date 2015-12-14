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
#include "../../../src/chisa/geom/Vector.hpp"
#include "../../../src/chisa/geom/Area.hpp"
#include <math.h>

namespace chisa {
namespace geom {

TEST(VectorTest, BasicTest)
{
	Box box1(1,2);
	Box box2(1,2.5);
	ASSERT_FLOAT_EQ(box1.width(),1);
	ASSERT_FLOAT_EQ(box1.height(),2);

	Point p1(1,2);
	Point p2(1,2.5);
	ASSERT_FLOAT_EQ(p1.x(),1);
	ASSERT_FLOAT_EQ(p1.y(),2);

	ASSERT_TRUE(p1.near(p2, 1));

	Point p3(Point(1,2)); //rvalue
	ASSERT_FLOAT_EQ(p3.x(),1);
	ASSERT_FLOAT_EQ(p3.y(),2);
}

TEST(VectorTest, FloatMDTest)
{
	{
		Box box1(1,2);
		Box box2 = box1/2;
		ASSERT_FLOAT_EQ(box2.x(), 0.5f);
		ASSERT_FLOAT_EQ(box2.y(), 1.0f);
	}
	{
		Box box1(1,2);
		Box box2 = box1*2;
		ASSERT_FLOAT_EQ(box2.x(), 2.0f);
		ASSERT_FLOAT_EQ(box2.y(), 4.0f);
	}
}

TEST(VectorTest, ScaleVectorTest)
{
	{
		Box box1(1,2);
		ScaleVector sv(3,4);
		Box box2(box1*sv);
		ASSERT_FLOAT_EQ(box2.x(), 3);
		ASSERT_FLOAT_EQ(box2.y(), 8);
	}
	{
		Box box1(1,2);
		ScaleVector sv(3,4);
		Box box2(box1/sv);
		ASSERT_FLOAT_EQ(box2.x(), 1.0/3);
		ASSERT_FLOAT_EQ(box2.y(), 2.0/4);
	}
}
#define OPTEST(V1,V2,OP,V3)\
do{\
	Vector v3 (V1 OP V2);\
	Vector v(V1);\
	v OP##= V2;\
	ASSERT_FLOAT_EQ(V3.x(), v3.x());\
	ASSERT_FLOAT_EQ(V3.y(), v3.y());\
	ASSERT_FLOAT_EQ(V3.x(), v.x());\
	ASSERT_FLOAT_EQ(V3.y(), v.y());\
}while(0);\

#define UNARY_TEST(OP,V1,V2)\
do{\
	Vector v2 (OP V1);\
	ASSERT_FLOAT_EQ(V2.x(), v2.x());\
	ASSERT_FLOAT_EQ(V2.y(), v2.y());\
}while(0);\

TEST(VectorTest, OpTest)
{
	OPTEST(Vector(1,2),Vector(3,4),+,Vector(4,6));
	OPTEST(Vector(1,2),Vector(3,4),-,Vector(-2,-2));

	UNARY_TEST(+, Vector(1,2),Vector(1,2));
	UNARY_TEST(-, Vector(1,2),Vector(-1,-2));

	UNARY_TEST(+, Vector(-1,2),Vector(-1,2));
	UNARY_TEST(-, Vector(-1,2),Vector(1,-2));

	UNARY_TEST(+, Vector(-1,-2),Vector(-1,-2));
	UNARY_TEST(-, Vector(-1,-2),Vector(1,2));
}


}}
