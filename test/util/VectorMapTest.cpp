/**
 * Saccubus
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

#include "../TestCommon.h"
#include <tarte/VectorMap.h>

#include <cmath>

namespace tarte {

typedef VectorMap<int, int> Vec;
typedef VectorMap<int, int>::Pair Pair;

TEST(VectorMapTest, EmptyTest)
{
	Vec v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(0, v.size());
	ASSERT_TRUE(v.end() == v.find(10));
}

TEST(VectorMapTest, InsertTest)
{
	Vec v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(0, v.size());
	v.insert(10, 1);
	ASSERT_TRUE(v.end() != v.find(10));
	Pair const& p = *(v.find(10));
	ASSERT_EQ(1, p.second);
}

TEST(VectorMapTest, UpdateTest)
{
	Vec v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(0, v.size());
	ASSERT_FALSE( v.update(10, 1) );
	ASSERT_TRUE( v.update(10, 1) );
	Pair const& p = *(v.find(10));
	ASSERT_EQ(1, p.second);
	ASSERT_TRUE( v.update(10, 2) );
	Pair const& p2 = *(v.find(10));
	ASSERT_EQ(2, p2.second);
	ASSERT_EQ(1, v.size());
}

TEST(VectorMapTest, RemoveTest)
{
	Vec v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(0, v.size());
	ASSERT_FALSE(v.remove(10));
	v.insert(10, 1);
	ASSERT_TRUE(v.end() != v.find(10));
	Pair const& p = *(v.find(10));
	ASSERT_EQ(1, p.second);
	ASSERT_TRUE(v.have(10));
	ASSERT_TRUE(v.remove(10));
	ASSERT_FALSE(v.have(10));
	ASSERT_FALSE(v.remove(10));
}

TEST(VectorMapTest, EraseTest)
{
	Vec v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(0, v.size());
	ASSERT_FALSE(v.remove(10));
	v.insert(10, 1);
	v.insert(11, 2);
	ASSERT_TRUE(v.end() != v.find(10));
	{
		Pair const& p = *(v.find(10));
		ASSERT_EQ(1, p.second);
	}
	{
		Pair const& p = *(v.erase( v.find(10) ));
		ASSERT_EQ(2, p.second);
	}
}

}
