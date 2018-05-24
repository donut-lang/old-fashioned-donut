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

TEST(UnitTest, StaticTest)
{
	constexpr Unit u;
	static_assert( std::is_trivial<decltype(u)>::value, "Unit must be trivial" );
}

}
