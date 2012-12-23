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

#include "DonutHelper.h"
#include <math.h>

namespace donut {

TEST(AssignOperationTest, NullTest)
{
	EXECUTE_SRC("");
	ASSERT_TRUE(result->isNull());
	ASSERT_FALSE(result->isObject());
}

TEST(AssignOperationTest, AssignTest)
{
	SOURCE_TEST_INT(1, "test=1;");
	SOURCE_TEST_INT(1, "t={test=>0};t.test=1;");
}

TEST(AssignOperationTest, ObjectTest)
{
	EXECUTE_SRC("{a=>1};");
	ASSERT_TRUE(result->has(heap, "a"));
	ASSERT_TRUE(result->isObject());
	ASSERT_EQ(1, result->get(heap, "a")->toInt(heap));
}

TEST(AssignOperationTest, ArrayTest)
{
	EXECUTE_SRC("[2,3,1];");
	ASSERT_TRUE(result->has(heap, "0"));
	ASSERT_EQ(2, result->get(heap, "0")->toInt(heap));
	ASSERT_EQ(2, result->get(heap, 0)->toInt(heap));

	ASSERT_TRUE(result->has(heap, "1"));
	ASSERT_EQ(3, result->get(heap, "1")->toInt(heap));
	ASSERT_EQ(3, result->get(heap, 1)->toInt(heap));

	ASSERT_TRUE(result->has(heap, "2"));
	ASSERT_EQ(1, result->get(heap, 2)->toInt(heap));
}

TEST(AssignOperationTest, AddTest)
{
	SOURCE_TEST_INT(3, "1+2;")
}

TEST(AssignOperationTest, AddTripleTest)
{
	SOURCE_TEST_INT(4, "2+1+1;")
}

TEST(AssignOperationTest, AssignOpTest)
{
	SOURCE_TEST_INT(1, "test=0; test+=1;");
	SOURCE_TEST_INT(1, "test=0; test+=1; test;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; t.test+=1;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; t.test+=1; t.test;");
}

TEST(AssignOperationTest, PostOpTest)
{
	SOURCE_TEST_INT(0, "test=0; test++;");
	SOURCE_TEST_INT(1, "test=0; test++; test;");
	SOURCE_TEST_INT(0, "t={}; t.test=0; t.test++;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; t.test++; t.test;");
	SOURCE_TEST_INT(0, "t={}; i=\"t\"; t[i]=0; t[i]++;");
	SOURCE_TEST_INT(1, "t={}; i=\"t\"; t[i]=0; t[i]++; t[i];");
	SOURCE_TEST_INT(0, "t=['1']; i=\"t\"; t[i]=0; t[i]++;");
	SOURCE_TEST_INT(1, "t=['1']; i=\"t\"; t[i]=0; t[i]++; t[i]");
}

TEST(AssignOperationTest, PreOpTest)
{
	SOURCE_TEST_INT(1, "test=0; ++test;");
	SOURCE_TEST_INT(1, "test=0; ++test; test;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; ++t.test;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; ++t.test; t.test;");
	SOURCE_TEST_INT(1, "t={}; i=\"t\"; t[i]=0; ++t[i];");
	SOURCE_TEST_INT(1, "t={}; i=\"t\"; t[i]=0; ++t[i]; t[i];");
	SOURCE_TEST_INT(1, "t=['1']; i=\"t\"; t[i]=0; ++t[i];");
	SOURCE_TEST_INT(1, "t=['1']; i=\"t\"; t[i]=0; ++t[i]; t[i];");
}

TEST(DonutRunTest, AssignOpTest)
{
	SOURCE_TEST_INT(1, "test=0; test+=1;");
	SOURCE_TEST_INT(1, "test=0; test+=1; test;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; t.test+=1;");
	SOURCE_TEST_INT(1, "t={}; t.test=0; t.test+=1; t.test;");
	SOURCE_TEST_INT(1, "t={}; i=\"t\"; t[i]=0; t[i]+=1;");
	SOURCE_TEST_INT(1, "t={}; i=\"t\"; t[i]=0; t[i]+=1; t[i];");
	SOURCE_TEST_INT(1, "t=['1']; i=\"t\"; t[i]=0; t[i]+=1;");
	SOURCE_TEST_INT(1, "t=['1']; i=\"t\"; t[i]=0; t[i]+=1; t[i];");
}

}
