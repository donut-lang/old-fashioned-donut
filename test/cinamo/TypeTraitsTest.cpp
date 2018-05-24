/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/TypeTraits.h>

namespace cinamo {

TEST(TypeTraitsTest, BasicTest)
{
	static_assert(
			std::is_same<
			typename ::cinamo::FuncTypeOf<decltype(&::isalnum)>::Result,
			decltype(::isalnum(1))
			>::value, "Could not return type.");
	static_assert(
			std::is_same<
			typename ::cinamo::FuncTypeOf<decltype(&::isalnum)>::Arg<0>::Type,
			int
			>::value, "Could not return type.");
	auto f = [](int x, double y, std::string z, float w){};
	static_assert(
			std::is_same<
			typename ::cinamo::FuncTypeOf<decltype(f)>::Arg<3>::Type,
			float
			>::value, "Could not return type.");
}

}
