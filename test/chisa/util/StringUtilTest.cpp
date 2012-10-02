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

}}
