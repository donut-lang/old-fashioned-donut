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
#include "DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {

TEST(DonutRunTest, NullTest)
{
	EXECUTE_SRC("");
	ASSERT_TRUE(result->isNull());
	ASSERT_FALSE(result->isObject());
}

TEST(DonutRunTest, StringTest)
{
	SOURCE_TEST_STR("abc", "'abc';");
}

TEST(DonutRunTest, AssignTest)
{
	SOURCE_TEST_INT(1, "test=1;");
}

TEST(DonutRunTest, ObjectTest)
{
	EXECUTE_SRC("test=1;");
	ASSERT_TRUE(result->have(&world, "a"));
	ASSERT_TRUE(result->isObject());
	ASSERT_EQ(1, result->load(&world, "a")->toInt(&world));
}

TEST(DonutRunTest, ArrayTest)
{
	EXECUTE_SRC("[2,3,1];");
	ASSERT_TRUE(result->have(&world, "0"));
	ASSERT_EQ(2, result->load(&world, "0")->toInt(&world));
	ASSERT_EQ(2, result->load(&world, 0)->toInt(&world));

	ASSERT_TRUE(result->have(&world, "1"));
	ASSERT_EQ(3, result->load(&world, "1")->toInt(&world));
	ASSERT_EQ(3, result->load(&world, 1)->toInt(&world));

	ASSERT_TRUE(result->have(&world, "2"));
	ASSERT_EQ(1, result->load(&world, 2)->toInt(&world));
}

TEST(DonutRunTest, AddTest)
{
	SOURCE_TEST_INT(3, "1+2;")
}

TEST(DonutRunTest, AddTripleTest)
{
	SOURCE_TEST_INT(4, "2+1+1;")
}

TEST(DonutRunTest, AssignOpTest)
{
	SOURCE_TEST_INT(1, "test=0; test+=1;")
}

TEST(DonutRunTest, PostOpTest)
{
	SOURCE_TEST_INT(0, "test=0; test++;");
	SOURCE_TEST_INT(1, "test=0; test++; test;");
}

TEST(DonutRunTest, PreOpTest)
{
	SOURCE_TEST_INT(1, "test=0; ++test;");
	SOURCE_TEST_INT(1, "test=0; ++test; test;");
}

}}


