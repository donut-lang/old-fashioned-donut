/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/FileSystem.h>

namespace cinamo {

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
