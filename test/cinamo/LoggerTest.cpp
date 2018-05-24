/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Logger.h>

namespace cinamo {

TEST(LoggerTest, LoggerFlagTest)
{
	Logger ll(std::cout, Logger::DEBUG_);
	ASSERT_FALSE(ll.t());
	ASSERT_FALSE(ll.v());
	ASSERT_TRUE(ll.d());
	ASSERT_TRUE(ll.i());
}

TEST(LoggerTest, LoggerOutTest)
{
	Logger ll(std::cout, Logger::DEBUG_);
	ASSERT_FALSE(ll.t("tag","example"));
	ASSERT_FALSE(ll.v("tag","example"));
	ASSERT_TRUE(ll.d("tag","example"));
	ASSERT_TRUE(ll.i("tag","example"));
	ASSERT_TRUE(ll.w("tag","example"));
	ASSERT_TRUE(ll.e("tag","example"));
}

}
