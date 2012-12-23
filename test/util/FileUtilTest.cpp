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
#include <tarte/FileSystem.h>

namespace tarte {

TEST(FileUtilTest, FileEnumTest)
{
	std::string dir=file::join(MATERIAL_DIR, "fileenum");
	std::vector<std::string> vec = file::enumFiles(dir, false);
	std::set<std::string> list(vec.begin(), vec.end());
	ASSERT_EQ(3, list.size());
	ASSERT_TRUE(list.find(file::join(dir,"a")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"b")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"c")) != list.end());

	vec = file::enumFiles(dir, true);
	list = std::set<std::string>(vec.begin(), vec.end());
	ASSERT_EQ(6, list.size());
	ASSERT_TRUE(list.find(file::join(dir,"a")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"b")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"c")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"d","x")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"d","y")) != list.end());
	ASSERT_TRUE(list.find(file::join(dir,"d","z")) != list.end());
}

TEST(FileUtilTest, MultiByteFileNameEnumTest)
{
	std::string dir=file::join(MATERIAL_DIR, "mb_file");
	std::vector<std::string> vec = file::enumFiles(dir, true);
	std::set<std::string> list(vec.begin(), vec.end());
	ASSERT_EQ(1, list.size());
	ASSERT_TRUE(list.find(file::join(dir,u8"あ")) != list.end());
}

TEST(FileUtilTest, JoinTest)
{
	using namespace file::internal;
	ASSERT_EQ(std::string("a")+FileConstants<std::string>::Sep+"b", file::join("a","b"));
	ASSERT_EQ(std::string("a")+FileConstants<std::string>::Sep+FileConstants<std::string>::Sep+"b", file::join(std::string("a")+FileConstants<std::string>::Sep,"b"));
}

}
