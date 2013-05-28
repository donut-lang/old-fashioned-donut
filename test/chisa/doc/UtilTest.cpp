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
#include "../../chisa/doc/Util.h"

namespace chisa {
namespace doc {

TEST(DocumentUtilTest, ShrinkSpaceTest)
{
	ASSERT_EQ("あい う", shrinkSpace("あい う"));
	ASSERT_EQ("あい う", shrinkSpace("あい  う"));
	ASSERT_EQ("あい う", shrinkSpace("あい                             う"));
	ASSERT_EQ("あい　う", shrinkSpace("あい　う"));
	ASSERT_EQ("あい　　　　　う", shrinkSpace("あい　　　　　う"));
	ASSERT_EQ("あ　 い う", shrinkSpace("あ　	い 		 	う"));
	ASSERT_EQ(" あ　 い う ", shrinkSpace("   	 	 	あ　	い 		 	う		 	 	"));
	ASSERT_EQ(" ", shrinkSpace("    "));
	ASSERT_EQ(" ", shrinkSpace("  \t  "));
}

TEST(DocumentUtilTest, EnglishNextStringTest)
{
	{
		std::vector<std::string> list(breakLine("abcde"));
		ASSERT_EQ(1, list.size());
	}
	{
		std::vector<std::string> list(breakLine("abc de"));
		ASSERT_EQ(2, list.size());
	}
}

TEST(DocumentUtilTest, JapaneseNextStringTest)
{
	{
		std::vector<std::string> list(breakLine("あいう"));
		ASSERT_EQ(3, list.size());
	}
	{
		std::vector<std::string> list(breakLine("あ"));
		ASSERT_EQ(1, list.size());
	}
	{
		std::vector<std::string> list(breakLine("あ。"));
		ASSERT_EQ(1, list.size());
	}
	{
		std::vector<std::string> list(breakLine("あ。いう。"));
		ASSERT_EQ(3, list.size());
	}
}

}}
