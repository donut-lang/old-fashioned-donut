/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/String.h>

#include <cfloat>
#include <cmath>
namespace cinamo {

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
	ASSERT_EQ("あ", (std::string&)chars[0]);
	ASSERT_EQ("い", (std::string&)chars[1]);
	ASSERT_EQ("う", (std::string&)chars[2]);
	ASSERT_EQ("え", (std::string&)chars[3]);
	ASSERT_EQ("お", (std::string&)chars[4]);
}

TEST(StringUtilTest, MatchString)
{
	ASSERT_EQ(3, matchString("abc","abc"));
	ASSERT_EQ(3, matchString("abc","abcdef"));
	ASSERT_EQ(3, matchString("abcdef","abc"));
	ASSERT_EQ(2, matchString("abc","abz"));
	ASSERT_EQ(0, matchString("abc","xyz"));
}

TEST(StringUtilTest, Join)
{
	std::vector<std::string> chars = {"あ","い","う",""};
	ASSERT_EQ("あいう", join(chars));
	ASSERT_EQ("あvいvうv", join(chars, "v"));
}

#define PARSE_TEST(str, val, type) \
{\
	type r;\
	ASSERT_TRUE(tryParseAs(str, r));\
	ASSERT_EQ(val, r);\
	ASSERT_EQ(val, parseAs<type>(str, -1));\
	ASSERT_EQ(val, parseAs("", r));\
}

#define PARSE_TEST_R(str, val, type, radix) \
{\
	type r;\
	ASSERT_TRUE(tryParseAs(str, radix, r));\
	ASSERT_EQ(val, r);\
	ASSERT_EQ(val, parseAs<type>(str, radix, -1));\
	ASSERT_EQ(val, parseAs<type>("", radix, r));\
}
TEST(StringUtilTest, ParseIntTest)
{
	PARSE_TEST("10", 10, int);
	PARSE_TEST_R("10", 10, int, 0);
	PARSE_TEST("0x10", 16, int);
	PARSE_TEST_R("0x10", 16, int, 0);
	PARSE_TEST("010", 8, int);
	PARSE_TEST_R("010", 8, int, 0);

	PARSE_TEST_R("10", 10, int, 10);
	PARSE_TEST_R("10", 8, int, 8);
	PARSE_TEST_R("10", 16, int, 16);
}
#undef PARSE_TEST_R

#define PARSE_TEST_F(str, val, type) \
{\
	type r;\
	ASSERT_TRUE(tryParseAs(str, r));\
	ASSERT_FLOAT_EQ(val, r);\
	ASSERT_FLOAT_EQ(val, parseAs<type>(str, r));\
	ASSERT_FLOAT_EQ(val, parseAs<type>("", r));\
}
TEST(StringUtilTest, ParseFloatTest)
{
	PARSE_TEST_F("10", 10.0f, float);
	PARSE_TEST_F("10", 10.0, double);
	PARSE_TEST_F("10.12", 10.12f, float);
	PARSE_TEST_F("10.12", 10.12, double);
}
TEST(StringUtilTest, ParseFloatMaximumTest)
{
	// マクロを展開すると、どうしてもドーナッツのリテラルと整合性が取れるかがわからないので、手打ち。悲しい。
	PARSE_TEST_F("3.402823466e+38", 3.402823466e+38, float);
	PARSE_TEST_F("1.17549e-38", 1.17549e-38, float);
	PARSE_TEST_F("33554431", 33554431, float);

	PARSE_TEST_F("1.7976931348623158e+308", 1.7976931348623158e+308, double);
	PARSE_TEST_F("2.2250738585072014e-308", 2.2250738585072014e-308, double);
	PARSE_TEST_F("18014398509481983.0", 18014398509481983.0, double);

	PARSE_TEST_F("1.18973e+4932", 1.18973e+4932L, long double);
	PARSE_TEST_F("3.3621e-4932", 3.3621e-4932L, long double);
	PARSE_TEST_F("10384593717069655257060992658440191.0", 10384593717069655257060992658440191.0, long double);
}
#undef PARSE_TEST_F

TEST(StringUtilTest, ParseBoolTest)
{
	ASSERT_TRUE(parseAs<bool>("true", false));
	ASSERT_TRUE(parseAs<bool>("True", false));
	ASSERT_TRUE(parseAs<bool>("yes", false));
	ASSERT_TRUE(parseAs<bool>("Yes", false));

	ASSERT_FALSE(parseAs<bool>("false", true));
	ASSERT_FALSE(parseAs<bool>("False", true));
	ASSERT_FALSE(parseAs<bool>("no", true));
	ASSERT_FALSE(parseAs<bool>("No", true));
}


TEST(StringUtilTest, NumericTest)
{
	ASSERT_EQ("123", format("%d", 123));
	ASSERT_EQ("123", format("%llu", 123LLU));
	ASSERT_EQ("123", format("%lld", 123LL));
	ASSERT_EQ("-123", format("%lld", -123LL));
	ASSERT_EQ("12.3", format("%.1f", 12.3));
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

TEST(StringUtilTest, TrimTest)
{
	ASSERT_EQ("abcdef", trim("abcdef"));

	ASSERT_EQ("abcdef", trim(" abcdef "));

	ASSERT_EQ("abcdef", trim("	  abcdef		  "));

	ASSERT_EQ("abcdef", trim(" 　abcdef 　"));
}

TEST(StringUtilTest, SplitTest)
{
	std::string front;
	std::string back;
	{
		std::vector<std::string> arg = split("you is a big fool man", " ");
		ASSERT_EQ(6U, arg.size());
		front = arg.front();
		back = arg.back();
		ASSERT_STREQ(front.c_str(), "you");
		ASSERT_STREQ(back.c_str(), "man");
	}

	{
		std::vector<std::string> arg = split("you  is a big fool man", " ");
		ASSERT_EQ(6U, arg.size());
		std::string const& front = arg.front();
		std::string const& back = arg.back();
		ASSERT_STREQ(front.c_str(), "you");
		ASSERT_STREQ(back.c_str(), "man");
	}

	ASSERT_EQ(2U, split("you is a big fool man", " is ").size());

	ASSERT_EQ(6U, splitSpace("you　is a　big fool man").size());

	ASSERT_EQ(6U, splitLine("you\r\nis\ra\nbig \r\n\r\n fo\nol man").size());

	ASSERT_EQ(0U, splitLine("").size());

	ASSERT_EQ(0U, splitLine("\r\n").size());

	ASSERT_EQ(0U, splitLine("\r").size());

	ASSERT_EQ(0U, splitSpace(" ").size());

	ASSERT_EQ(0U, splitSpace("").size());

	ASSERT_EQ(0U, split("", "").size());

	ASSERT_EQ(0U, split("", "a").size());
}

}
