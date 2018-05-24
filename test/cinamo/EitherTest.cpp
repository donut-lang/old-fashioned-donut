/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Base64.h>
#include <cinamo/functional/Either.h>

#include <cfloat>
#include <cmath>

namespace cinamo {

TEST(EitherTest, ConstexprBasicTest)
{
	constexpr Either<int, int> i = Right<int, int>(10);
	static_assert(i.isRight, "right is right");
	static_assert(!i.isLeft, "right is not left.");
	static_assert(i.answer() == 10, "does not have value.");
}

TEST(EitherTest, BindTest)
{
	Either<int, int> i = Right<int, int>(10);
	auto r = (i >>= [](int x)->Either<int, int>{return Right<int,int>(x+190);});

	ASSERT_TRUE(r.isRight);
	ASSERT_FALSE(r.isLeft);
	ASSERT_EQ(r.answer(), 200);
}

TEST(EitherTets, IfLeftTest)
{
	{
		Either<int, int> i = Right<int, int>(10);

		int v = 0;
		i.ifLeft([&](int i)->void{ v=1; });
		ASSERT_EQ(0, v);
		ASSERT_EQ(i, i.ifLeft([](int i) -> int{ return i+1; }));
	}
	{
		Either<int, int> i = Left<int, int>(10);

		int v = 0;
		i.ifLeft([&](int i)->void{ v=1; });
		ASSERT_EQ(1, v);
		ASSERT_EQ(i, i.ifLeft([](int i) -> int{ return i+1; }));
	}
}

TEST(EitherTets, IfRightTest)
{
	{
		Either<int, int> i = Right<int, int>(10);

		int v = 0;
		ASSERT_EQ(i,i.ifRight([&](int i)->void{ v=1; }));
		ASSERT_EQ(1, v);
	}
	{
		Either<int, int> i = Left<int, int>(10);

		int v = 0;
		ASSERT_EQ(i, i.ifRight([&](int i)->void{ v=1; }));
		ASSERT_EQ(0, v);
	}
}

TEST(EitherTest, Compare)
{
	ASSERT_TRUE((Right<int, int>(10).isRight));
	ASSERT_GT((Right<int,int>(10)).answer(), (Right<int,int>(9)).answer());

	ASSERT_EQ((Right<int,int>(10)), (Right<int,int>(10)));
	ASSERT_NE((Right<int,int>(10)), (Right<int,int>(-10)));

	ASSERT_NE((Right<int,int>(10)), (Left<int,int>(10)));
	ASSERT_NE((Left<int,int>(10)), (Right<int,int>(10)));

	ASSERT_EQ((Left<int,int>(10)), (Left<int,int>(10)));
	ASSERT_NE((Left<int,int>(10)), (Left<int,int>(-10)));
}

TEST(EitherTest, CompareWithDifferentTypes)
{
	//関係ないタイプ同士で異なる
	ASSERT_NE((Right<int,int>(10)), (Right<int,long>(10)));
	ASSERT_NE((Right<int,long>(10)), (Right<int,int>(10)));

	ASSERT_EQ((Right<int,int>(10)), (Right<long,int>(10)));
	ASSERT_EQ((Right<long,int>(10)), (Right<int,int>(10)));

	ASSERT_NE((Left<int,int>(10)), (Left<long,int>(10)));
	ASSERT_NE((Left<long,int>(10)), (Left<int,int>(10)));

	ASSERT_EQ((Left<int,int>(10)), (Left<int,long>(10)));
	ASSERT_EQ((Left<int,long>(10)), (Left<int,int>(10)));
}
}
