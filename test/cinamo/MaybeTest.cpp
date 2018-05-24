/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Base64.h>
#include <cinamo/functional/Maybe.h>

#include <cfloat>
#include <cmath>

namespace cinamo {

TEST(MaybeTest, NothingTest)
{
	constexpr Maybe<int> x = Nothing<int>();
	ASSERT_TRUE(x.isNothing);
	ASSERT_FALSE(x.isJust);

	auto f = x >>= [](int x){ return Just<int>(x+1); };
	static_assert(std::is_same<decltype(f), Maybe<int> >::value, "f must be Maybe<int>");
	ASSERT_TRUE(f.isNothing);
	ASSERT_FALSE(f.isJust);
}
TEST(MaybeTest, JustTest)
{
	constexpr Maybe<int> x = Just<int>(10);
	ASSERT_TRUE(x.isJust);
	ASSERT_FALSE(x.isNothing);
	ASSERT_EQ(10, x.value());

	auto f = x >>= [](int x){ return Just<int>(x+1); };
	static_assert(std::is_same<decltype(f), Maybe<int> >::value, "f must be Maybe<int>");
	ASSERT_TRUE(f.isJust);
	ASSERT_FALSE(f.isNothing);
	ASSERT_EQ(11, f.value());
}

TEST(MaybeTest, Compare)
{
	ASSERT_EQ(Just(1), Just(1));
	ASSERT_NE(Just(1), Just(2));
	ASSERT_NE(Just<int>(1), Just<long>(1));

	ASSERT_EQ(Nothing<int>(), Nothing<int>());
	ASSERT_NE(Nothing<int>(), Nothing<unsigned int>());
}

}
