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

#include "../../TestCommon.hpp"
#include <cinamo/Exception.h>

namespace chisa {
namespace logging{

TEST(ExceptionTest, ThrowingTest)
{
	ASSERT_THROW(throw Exception(__FILE__, __LINE__, "Hey!"), Exception);
}

TEST(ExceptionTest, MessageTest)
{
	std::string src = __FILE__;
	std::size_t line = __LINE__;
	Exception e(__FILE__, __LINE__, "Hey!");
	ASSERT_EQ("Hey!", e.msg());
	ASSERT_EQ(src, e.file());
	ASSERT_EQ(line+1, e.line());
	ASSERT_FALSE(std::string(e.what()).empty());

	src = __FILE__;
	line = __LINE__;
	Exception e2(__FILE__, __LINE__, "%d", 12);
	ASSERT_EQ("12", e2.msg());
	ASSERT_EQ(src, e2.file());
	ASSERT_EQ(line+1, e2.line());
	ASSERT_FALSE(std::string(e2.what()).empty());
}

TEST(ExceptionTest, EmptyTest)
{
	std::string src = __FILE__;
	std::size_t line = __LINE__;
	Exception e(__FILE__, __LINE__);
	ASSERT_TRUE(e.msg().empty());
	ASSERT_EQ(src, e.file());
	ASSERT_EQ(line+1, e.line());
	ASSERT_FALSE(std::string(e.what()).empty());

}

}}
