/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/VectorMap.h>

#include <cmath>

namespace cinamo {

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
