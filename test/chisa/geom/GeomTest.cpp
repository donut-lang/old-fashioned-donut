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
#include <math.h>

namespace chisa {
namespace geom {

TEST(GeomTest, BasicTest)
{
	Area area1(1,2,3,4);
	Area area2(1,2,3,4.5);
	ASSERT_FLOAT_EQ(area1.x(), 1);
	ASSERT_FLOAT_EQ(area1.y(), 2);
	ASSERT_FLOAT_EQ(area1.width(), 3);
	ASSERT_FLOAT_EQ(area1.height(), 4);
	ASSERT_TRUE(area1.near(area2, 1));

	area1.x(10);
	area1.y(11);
	area1.width(12);
	area1.height(13);
	ASSERT_FLOAT_EQ(area1.x(), 10);
	ASSERT_FLOAT_EQ(area1.y(), 11);
	ASSERT_FLOAT_EQ(area1.width(), 12);
	ASSERT_FLOAT_EQ(area1.height(), 13);

	area1.box().width(0);
	area1.box().height(0);
	ASSERT_FLOAT_EQ(area1.width(), 0);
	ASSERT_FLOAT_EQ(area1.height(), 0);
	ASSERT_TRUE(area1.empty());

	Box box1(1,2);
	Box box2(1,2.5);
	ASSERT_FLOAT_EQ(box1.width(),1);
	ASSERT_FLOAT_EQ(box1.height(),2);

	Point p1(1,2);
	Point p2(1,2.5);
	ASSERT_FLOAT_EQ(p1.x(),1);
	ASSERT_FLOAT_EQ(p1.y(),2);

	ASSERT_TRUE(p1.near(p2, 1));
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

TEST(GeomTest, OpTest)
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


Area testIntersect(Area x, Area y)
{
	return x.intersect(y);
}

TEST(GeomTest, IntersectTest)
{
	ASSERT_TRUE(testIntersect(Area(0,0,100,100), Area(0,0,100,100)).near(Area(0,0,100,100),.5));
	ASSERT_TRUE(testIntersect(Area(0,0,100,50), Area(0,0,100,100)).near(Area(0,0,100,50),.5));
	ASSERT_TRUE(testIntersect(Area(99,98,100,100), Area(0,0,100,100)).near(Area(99,98,1,2),.5));
	ASSERT_TRUE(testIntersect(Area(50,50,25,25), Area(0,0,100,100)).near(Area(50,50,25,25),.5));
	ASSERT_TRUE(testIntersect(Area(50,50,50,50), Area(0,0,100,100)).near(Area(50,50,50,50),.5));

	ASSERT_TRUE(testIntersect(Area(100,0,100,100), Area(0,0,100,100)).empty());
	ASSERT_TRUE(testIntersect(Area(0,100,100,100), Area(0,0,100,100)).empty());
	ASSERT_TRUE(testIntersect(Area(100,100,100,100), Area(0,0,100,100)).empty());
	ASSERT_TRUE(testIntersect(Area(100,100,0,0), Area(0,0,100,100)).empty());
}

}}

