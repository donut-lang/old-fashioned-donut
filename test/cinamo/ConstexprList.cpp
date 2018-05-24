/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/functional/List.h>
#include <cinamo/String.h>

namespace cinamo {

std::string to(int a, int b, int c){
	return format("%d %d %d", a, b, c);
}
TEST(ConstexprList, ApplyFunctionTest){
	List<int,3 > lst(1,2,3);

	ASSERT_EQ("1 2 3", lst.call(to));
}

TEST(ConstexprList, MakeListTest){
	auto lst = toList(1,2,3);

	ASSERT_EQ("1 2 3", lst.call(to));
}

TEST(ConstexprList, ConsTest){
	auto lst = toList(1,2,3);
	auto lst2 = lst.cons(0);

	ASSERT_EQ(0, lst2[0]);
	ASSERT_EQ(1, lst2[1]);
	ASSERT_EQ(2, lst2[2]);
	ASSERT_EQ(3, lst2[3]);
}

TEST(ConstexprList, AppendTest){
	auto lst1 = toList( 1, 2, 3);
	auto lst2 = toList(-1,-2,-3);
	auto lst = lst1.append(lst2);

	ASSERT_EQ(lst[0], 1);
	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst[2], 3);
	ASSERT_EQ(lst[3], -1);
	ASSERT_EQ(lst[4], -2);
	ASSERT_EQ(lst[5], -3);
}

TEST(ConstexprList, SubListTest){
	constexpr auto lst1 = toList( 1, 2, 3, 4, 5, 6);
	constexpr auto lst = lst1.sub<1, 2>();

	ASSERT_EQ(lst[0], 2);
	ASSERT_EQ(lst[1], 3);
}

}
