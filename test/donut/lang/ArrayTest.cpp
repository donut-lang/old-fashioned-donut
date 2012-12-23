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

TEST(ArrayTest, LiteralTest)
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

TEST(ArrayTest, LiteralAccessIndex)
{
	SOURCE_TEST_FLOAT(1.0, "array=['0',1.0,2];array[1];");
}

TEST(ArrayTest, LiteralAssignIndex)
{
	SOURCE_TEST_STR("4", "array=['0',1.0,2];array[2]='4';");
}

TEST(ArrayTest, LiteralAssignAndGetIndex)
{
	SOURCE_TEST_STR("4", "array=['0',1.0,2];array[2]='4';array[2];");
}

TEST(ArrayTest, IntIndex)
{
	SOURCE_TEST_INT(3, "array={};array[1]=1;array[2]=2;array[3]=3;");
	SOURCE_TEST_INT(3, "array={};array[1]=1;array[2]=2;array[3]=3;array[3]");
}

TEST(ArrayTest, StringIndex)
{
	SOURCE_TEST_INT(3, "array={};array['a']=1;array['b']=2;array['c']=3;");
	SOURCE_TEST_INT(2, "array={};array['a']=1;array['b']=2;array['c']=3;array['b'];");
}

}
