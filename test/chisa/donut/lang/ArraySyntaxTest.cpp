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

namespace chisa {
namespace donut {

TEST(ArraySyntaxTest, LiteralAccessIndex)
{
	SOURCE_TEST_FLOAT(1.0, "array=['0',1.0,2];array[2];");
}

TEST(ArraySyntaxTest, LiteralAssignIndex)
{
	SOURCE_TEST_STR("4", "array=['0',1.0,2];array[2]='4';");
}

TEST(ArraySyntaxTest, LiteralAssignAndGetIndex)
{
	SOURCE_TEST_STR("4", "array=['0',1.0,2];array[2]='4';array[2];");
}

TEST(ArraySyntaxTest, IntIndex)
{
	SOURCE_TEST_INT(3, "array={};array[1]=1;array[2]=2;array[3]=3;");
	SOURCE_TEST_INT(3, "array={};array[1]=1;array[2]=2;array[3]=3;array[3]");
}

TEST(ArraySyntaxTest, StringIndex)
{
	SOURCE_TEST_INT(3, "array={};array['a']=1;array['b']=2;array['c']=3;");
	SOURCE_TEST_INT(2, "array={};array['a']=1;array['b']=2;array['c']=3;array['b'];");
}

}}


