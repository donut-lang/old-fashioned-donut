/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/String.h>
#include <cinamo/Exception.h>

namespace cinamo {

TEST(ExceptionTest, GetSetTest)
{
	{
		cinamo::Exception e("a", 0);
		ASSERT_EQ( e.line(), 0);
		ASSERT_EQ( e.file(), "a" );
		ASSERT_EQ( e.msg(), "");
		ASSERT_GT( std::string(e.what()).length(), 0 );
	}
	{
		cinamo::Exception e("a", 0, "%d", 10);
		ASSERT_EQ( e.line(), 0);
		ASSERT_EQ( e.file(), "a" );
		ASSERT_EQ( e.msg(), "10");
		ASSERT_GT( std::string(e.what()).length(), 0 );
	}
}
TEST(ExceptionTest, ThrowTest)
{
	try {
		CINAMO_EXCEPTION(Exception, "%d", 1)
	} catch (Exception& e) {
		ASSERT_EQ( e.line(), __LINE__ - 2 );
		ASSERT_EQ( e.file(), __FILE__ );
		ASSERT_EQ( e.msg(), "1");
		ASSERT_GT( std::string(e.what()).length(), 0 );
	}
}

}
