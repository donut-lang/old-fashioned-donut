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

#include "DonutHelper.hpp"
#include <math.h>

namespace donut {

TEST(StringTest, ImplicitConvertTest)
{
	INIT_DONUT;
	ASSERT_ANY_THROW( heap->createStringObject("test")->toBool(heap) );
	ASSERT_ANY_THROW( heap->createStringObject("test")->toFloat(heap) );
	ASSERT_ANY_THROW( heap->createStringObject("test")->toInt(heap) );
	ASSERT_NO_THROW( heap->createStringObject("test")->toString(heap) );
}

TEST(StringTest, SingleLiteralTest)
{
	SOURCE_TEST_STR("abc", "'abc';");
}
TEST(StringTest, DoubleLiteralTest)
{
	SOURCE_TEST_STR("abc", "\"abc\";");
}

TEST(StringTest, ConnectTest)
{
	SOURCE_TEST_STR("abcdef", "\"abc\"+\"def\";");
}

TEST(StringTest, MultTest)
{
	SOURCE_TEST_STR("abcabcabcabc", "\"abc\"*4;");
}

TEST(StringTest, MultAssignTest)
{
	SOURCE_TEST_STR("abcabcabcabc", "var test=\"abc\";test*=4;");
}

TEST(StringTest, ToIntegerTest)
{
	SOURCE_TEST_INT(123, "\"123\".toInteger();");
}

TEST(StringTest, ToIntegerHexTest)
{
	SOURCE_TEST_INT(0x123, "\"0x123\".toInteger();");
}

TEST(StringTest, ToIntegerOctTest)
{
	SOURCE_TEST_INT(0123, "\"0123\".toInteger();");
}

TEST(StringTest, ToBooleanTrueFalseTest)
{
	SOURCE_TEST_TRUE("\"true\".toBoolean();");
	SOURCE_TEST_FALSE("\"false\".toBoolean();");
}

TEST(StringTest, ToBooleanTrueFalseBigTest)
{
	SOURCE_TEST_TRUE("\"True\".toBoolean();");
	SOURCE_TEST_FALSE("\"False\".toBoolean();");
}

TEST(StringTest, ToBooleanYesNoTest)
{
	SOURCE_TEST_TRUE("\"yes\".toBoolean();");
	SOURCE_TEST_FALSE("\"no\".toBoolean();");
}

TEST(StringTest, ToBooleanYesNoBigTest)
{
	SOURCE_TEST_TRUE("\"Yes\".toBoolean();");
	SOURCE_TEST_FALSE("\"No\".toBoolean();");
}

TEST(StringTest, Compare)
{
	std::string const str1 = "abcdef";
	std::string const str2 = "XYZabcc";

	//違う文字列同士
	ASSERT_TRUE((str1 > str2));
	ASSERT_TRUE((str1 >= str2));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\"> \""+str2+"\";"));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\">=\""+str2+"\";"));

	ASSERT_FALSE((str1 == str2));
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\"==\""+str2+"\";"));

	ASSERT_TRUE((str1 != str2));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\"!=\""+str2+"\";"));

	ASSERT_FALSE(str1 < str2);
	ASSERT_FALSE(str1 <= str2);
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\"< \""+str2+"\";"));
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\"<=\""+str2+"\";"));

	//同じ文字列同士
	ASSERT_FALSE((str1 < str1));
	ASSERT_FALSE((str2 < str2));
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\"<\""+str1+"\";"));
	SOURCE_TEST_FALSE((std::string("\"")+str2+"\"<\""+str2+"\";"));

	ASSERT_FALSE((str1 > str1));
	ASSERT_FALSE((str2 > str2));
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\">\""+str1+"\";"));
	SOURCE_TEST_FALSE((std::string("\"")+str2+"\">\""+str2+"\";"));

	ASSERT_TRUE((str1 >= str1));
	ASSERT_TRUE((str2 >= str2));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\">=\""+str1+"\";"));
	SOURCE_TEST_TRUE((std::string("\"")+str2+"\">=\""+str2+"\";"));

	ASSERT_TRUE((str1 <= str1));
	ASSERT_TRUE((str2 <= str2));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\"<=\""+str1+"\";"));
	SOURCE_TEST_TRUE((std::string("\"")+str2+"\"<=\""+str2+"\";"));

	ASSERT_TRUE((str1 == str1));
	ASSERT_TRUE((str2 == str2));
	SOURCE_TEST_TRUE((std::string("\"")+str1+"\"==\""+str1+"\";"));
	SOURCE_TEST_TRUE((std::string("\"")+str2+"\"==\""+str2+"\";"));

	ASSERT_FALSE((str1 != str1));
	ASSERT_FALSE((str2 != str2));
	SOURCE_TEST_FALSE((std::string("\"")+str1+"\"!=\""+str1+"\";"));
	SOURCE_TEST_FALSE((std::string("\"")+str2+"\"!=\""+str2+"\";"));
}

}
