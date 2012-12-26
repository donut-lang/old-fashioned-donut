/**
 * Saccubus
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

#include "../TestCommon.h"
#include <tarte/String.h>

#include <cfloat>
#include <cmath>

namespace tarte {

TEST(StringUtilTest, FormatTest)
{
	ASSERT_EQ("NOT_FORMATTED", format("NOT_FORMATTED"));
	ASSERT_EQ("test %%% test", format("test %%%%%% test"));
	ASSERT_EQ("test 12 test", format("test %d test", 12));
	ASSERT_STRCASEEQ("test c test", format("test %x test", 0xc).c_str());
	ASSERT_STRCASEEQ("test 123 test", format("test %o test", 0123).c_str());
	ASSERT_STRCASEEQ("test \t test", format("test %c test", '\t').c_str());
	ASSERT_STRCASEEQ("test \r test", format("test %c test", 0x0d).c_str());
	ASSERT_STRCASEEQ("test \n test", format("test %c test", 0x0a).c_str());
	ASSERT_EQ("test test test", format("test %s test", "test"));
	ASSERT_EQ("test  test", format("test %s test", ""));
}

TEST(StringUtilTest, ToStringIntegerTest)
{
	ASSERT_EQ("12", toString(12));
	ASSERT_EQ("12", toString(12, 0));
	ASSERT_EQ("12", toString(12, 10));

	ASSERT_EQ("014", toString(12, 8));
	ASSERT_EQ("0xc", toString(12, 16));

	ASSERT_EQ("2147483647", toString(2147483647U, 10));
	ASSERT_EQ("-2147483647", toString(-2147483647, 10));
	ASSERT_EQ("4294967295", toString(4294967295U, 10));

	ASSERT_EQ("0xffffffff", toString(4294967295U, 16));
	ASSERT_EQ("037777777777", toString(4294967295U, 8));
}

TEST(StringUtilTest, ToStringInteger64bitTest)
{
	ASSERT_EQ("429496729700", toString(429496729700U));
	ASSERT_EQ("-429496729700", toString(-429496729700));

	//最大値近辺
	ASSERT_EQ("18446744073709551615", toString(18446744073709551615U));
	ASSERT_EQ("-9223372036854775807", toString(-9223372036854775807));
	ASSERT_EQ("0xffffffffffffffff", toString(18446744073709551615U, 16));
	ASSERT_EQ("01777777777777777777777", toString(18446744073709551615U, 8));
}

TEST(StringUtilTest, ToStringFloatTest)
{
	ASSERT_TRUE(startsWith(toString(12.3f), "12.3"));
}
TEST(StringUtilTest, ToStringFloatNANTest)
{
	ASSERT_EQ(toLower(toString((float)NAN)), "nan");
}

TEST(StringUtilTest, ToStringBoolTest)
{
	ASSERT_EQ("true", toString(true));
	ASSERT_EQ("false", toString(false));
}

TEST(StringUtilTest, ToLowerTest)
{
	ASSERT_EQ("l o w e r ", toLower("L O w e R "));
}

TEST(StringUtilTest, ToUpperTest)
{
	ASSERT_EQ(" UPPER ", toUpper(" upper "));
}

TEST(StringUtilTest, BreakChar)
{
	std::vector<std::string> chars(breakChar("あいうえお"));
	ASSERT_EQ(5, chars.size());
	ASSERT_EQ("あ", chars[0]);
	ASSERT_EQ("い", chars[1]);
	ASSERT_EQ("う", chars[2]);
	ASSERT_EQ("え", chars[3]);
	ASSERT_EQ("お", chars[4]);
}

TEST(StringUtilTest, ParseIntTest)
{
	ASSERT_EQ(10, parseAs<int>("10"));
	ASSERT_EQ(10, parseAs<int>("10",0));
	ASSERT_EQ(16, parseAs<int>("0x10"));
	ASSERT_EQ(8, parseAs<int>("010"));
	ASSERT_EQ(10, parseAs<int>("10",10));
	ASSERT_EQ(8, parseAs<int>("10",8));
	ASSERT_EQ(16, parseAs<int>("10",16));
}

TEST(StringUtilTest, ParseFloatTest)
{
	ASSERT_FLOAT_EQ(10.0f, parseAs<float>("10"));
	ASSERT_FLOAT_EQ(10.12f, parseAs<float>("10.12"));
}

TEST(StringUtilTest, ParseFloatMaximumTest)
{
	// マクロを展開すると、どうしてもドーナッツのリテラルと整合性が取れるかがわからないので、手打ち。悲しい。
	ASSERT_FLOAT_EQ(3.402823466e+38, parseAs<float>("3.402823466e+38"));
	ASSERT_FLOAT_EQ(1.17549e-38, parseAs<float>("1.17549e-38"));
	ASSERT_FLOAT_EQ(33554431, parseAs<float>("33554431"));

	ASSERT_FLOAT_EQ(1.7976931348623158e+308, parseAs<double>("1.7976931348623158e+308"));
	ASSERT_FLOAT_EQ(2.2250738585072014e-308, parseAs<double>("2.2250738585072014e-308"));
	ASSERT_FLOAT_EQ(18014398509481983.0, parseAs<double>("18014398509481983.0"));

	ASSERT_FLOAT_EQ(1.18973e+4932L, parseAs<long double>("1.18973e+4932"));
	ASSERT_FLOAT_EQ(3.3621e-4932L, parseAs<long double>("3.3621e-4932"));
	ASSERT_FLOAT_EQ(10384593717069655257060992658440191.0L, parseAs<long double>("10384593717069655257060992658440191.0"));
}

TEST(StringUtilTest, ParseBoolTest)
{
	ASSERT_TRUE(parseAs<bool>("true"));
	ASSERT_TRUE(parseAs<bool>("True"));
	ASSERT_TRUE(parseAs<bool>("yes"));
	ASSERT_TRUE(parseAs<bool>("Yes"));

	ASSERT_FALSE(parseAs<bool>("false"));
	ASSERT_FALSE(parseAs<bool>("False"));
	ASSERT_FALSE(parseAs<bool>("no"));
	ASSERT_FALSE(parseAs<bool>("No"));
}


TEST(StringUtilTest, NumericTest)
{
	ASSERT_EQ("123", format("%d", 123));
	ASSERT_EQ("123", format("%llu", 123LLU));
	ASSERT_EQ("123", format("%lld", 123LL));
	ASSERT_EQ("-123", format("%lld", -123LL));
	ASSERT_EQ("12.3", format("%.1f", 12.3));
}

TEST(StringUtilTest, PercentDecodeTest)
{
	ASSERT_EQ("NOT_ENCODED", decodePercent("NOT_ENCODED"));
	ASSERT_EQ("test PERCENT_ENCOD\r\nED", decodePercent("test %50%45%52%43%45%4e%54%5f%45%4e%43%4f%44%0d%0a%45%44"));
}

TEST(StringUtilTest, StartsWithTest)
{
	ASSERT_TRUE(startsWith("abcdef","")); //この挙動はJavaと同じ
	ASSERT_TRUE(startsWith("abcdef","abc"));
	ASSERT_FALSE(startsWith("abcdef","def"));
	ASSERT_TRUE(startsWith("abcdef","abcdef"));
}

TEST(StringUtilTest, EndsWithTest)
{
	ASSERT_TRUE(endsWith("abcdef","")); //この挙動はJavaと同じ
	ASSERT_FALSE(endsWith("abcdef","abc"));
	ASSERT_TRUE(endsWith("abcdef","def"));
	ASSERT_TRUE(endsWith("abcdef","abcdef"));
}

TEST(StringUtilTest, SplitTest)
{
	std::vector<std::string> arg;
	std::string front;
	std::string back;
	split("you is a big fool man", " ", arg);
	ASSERT_EQ(6U, arg.size());
	front = arg.front();
	back = arg.back();
	ASSERT_STREQ(front.c_str(), "you");
	ASSERT_STREQ(back.c_str(), "man");
	arg.clear();

	split("you  is a big fool man", " ", arg);
	ASSERT_EQ(6U, arg.size());
	front = arg.front();
	back = arg.back();
	ASSERT_STREQ(front.c_str(), "you");
	ASSERT_STREQ(back.c_str(), "man");
	arg.clear();

	split("you is a big fool man", " is ", arg);
	ASSERT_EQ(2U, arg.size());
	arg.clear();

	splitSpace("you　is a　big fool man", arg);
	ASSERT_EQ(6U, arg.size());
	arg.clear();

	splitLine("you\r\nis\ra\nbig \r\n\r\n fo\nol man", arg);
	ASSERT_EQ(6U, arg.size());
	arg.clear();

	splitLine("", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	splitLine("\r\n", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	splitLine("\r", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	splitSpace(" ", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	splitSpace("", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	split("", "", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();

	split("", "a", arg);
	ASSERT_EQ(0U, arg.size());
	arg.clear();
}

}
