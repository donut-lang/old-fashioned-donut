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

#include "../../TestCommon.h"
#include "../../../src/chisa/util/StringUtil.h"
using namespace chisa::util;

#include <cmath>

namespace chisa {
namespace util {

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
}

TEST(StringUtilTest, ToStringFloatTest)
{
	ASSERT_TRUE(startsWith(toString(12.3f), "12.3"));
	ASSERT_EQ(toLower(toString(NAN)), "nan");
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

TEST(StringUtilTest, ParseIntTest)
{
	ASSERT_EQ(10, parseInt("10"));
	ASSERT_EQ(10, parseInt("10",0));
	ASSERT_EQ(16, parseInt("0x10"));
	ASSERT_EQ(8, parseInt("010"));
	ASSERT_EQ(10, parseInt("10",10));
	ASSERT_EQ(8, parseInt("10",8));
	ASSERT_EQ(16, parseInt("10",16));
}

TEST(StringUtilTest, ParseFloatTest)
{
	ASSERT_FLOAT_EQ(10.0f, parseFloat("10"));
	ASSERT_FLOAT_EQ(10.12f, parseFloat("10.12"));
}

TEST(StringUtilTest, ParseBoolTest)
{
	ASSERT_TRUE(parseBool("true"));
	ASSERT_TRUE(parseBool("True"));
	ASSERT_TRUE(parseBool("yes"));
	ASSERT_TRUE(parseBool("Yes"));

	ASSERT_FALSE(parseBool("false"));
	ASSERT_FALSE(parseBool("False"));
	ASSERT_FALSE(parseBool("no"));
	ASSERT_FALSE(parseBool("No"));
}


TEST(StringUtilTest, NumericTest)
{
	ASSERT_EQ("123", format("%d", 123));
	ASSERT_EQ("123", format("%llu", 123LLU));
	ASSERT_EQ("123", format("%lld", 123LL));
	ASSERT_EQ("-123", format("%lld", -123LL));
	ASSERT_EQ("12.3", format("%.1f", 12.3));
}


TEST(StringUtilTest, Base64EecodeTest)
{
	{
		std::vector<char> d;
		d.push_back('t');
		d.push_back('e');
		d.push_back('s');
		d.push_back('t');
		ASSERT_EQ("dGVzdA==", encodeBase64(d));
	}
	ASSERT_EQ("dGVzdA==", encodeBase64("test"));
	ASSERT_EQ("dGVzdA==", encodeBase64("test", 4));

	ASSERT_EQ("dA==", encodeBase64("t"));
	ASSERT_EQ("dGU=", encodeBase64("te"));
	ASSERT_EQ("dGVz", encodeBase64("tes"));
	ASSERT_EQ("dGVzdA==", encodeBase64("test"));

	ASSERT_EQ("44GC44GE44GG44GI44GK", encodeBase64("あいうえお"));
	{
		std::vector<char> d;
		d.push_back('t');
		d.push_back(0);
		d.push_back('s');
		d.push_back('t');
		d.push_back(0);
		d.push_back('t');
		d.push_back(21);
		d.push_back(0);
		d.push_back(255);
		d.push_back(42);
		ASSERT_EQ(d, decodeBase64("dABzdAB0FQD/Kg=="));
	}
}

TEST(StringUtilTest, Base64DecodeTest)
{
	std::vector<char> d;
	d.push_back('t');
	d.push_back('e');
	d.push_back('s');
	d.push_back('t');
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+1), decodeBase64("dA=="));
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+2), decodeBase64("dGU="));
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+3), decodeBase64("dGVz"));
	ASSERT_EQ(d, decodeBase64("dGVzdA=="));
	{
		std::vector<char> d;
		d.push_back('t');
		d.push_back(0);
		d.push_back('s');
		d.push_back('t');
		d.push_back(0);
		d.push_back('t');
		d.push_back(21);
		d.push_back(0);
		d.push_back(255);
		d.push_back(42);
		ASSERT_EQ("dABzdAB0FQD/Kg==", encodeBase64(d));
		ASSERT_EQ(d, decodeBase64(encodeBase64(d)));
	}
	ASSERT_EQ("test", decodeBase64AsString("dGVzdA=="));

	ASSERT_EQ("あいうえお", decodeBase64AsString("44GC44GE44GG44GI44GK"));
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

}}
