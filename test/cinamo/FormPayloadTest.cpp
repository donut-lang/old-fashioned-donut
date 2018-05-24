/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <fstream>
#include <istream>
#include <string>
#include <cinamo/FormPayload.h>

namespace cinamo{

static const std::string RPU_JSON="{\"count\":1595356,\"users\":[\"\\u3061\\u3073\\u7f8e\\u3002\",\"\\u308c\\u3093\\u3075\\u3041\",\"harutti92\",\"247 \\/ \\u4ec1\\u79d1\",\"nikola\",\"\\u4e9c\\u9f8d\",\"rc42\",\"\\u307e\\u3042\",\"\\u3081\\u3081\",\"shojikatyou\",\"\\u5929\\u6d25\\u7518\\u6817\",\"\\u30a2\\u30e9\\u30b8\\u30f3\\u30ca\\u30a4\\u30c8\",\"\\u3082\\u3081\\u3093\",\"sleep714\",\"\\u30e9\\u30de\",\"\\u3082\\u3063\\u3061\\u3083\\u3093\",\"\\u30ca\\u30eb\\u30b9\",\"Ryousuke\",\"ikaros\",\"\\u306f\\u308b\\u3054\\u3093\\u592a\\u90ce\"],\"extra\":18}";

TEST(FormPayloadTest, ParseTest)
{
	std::ifstream stream(MATERIAL_DIR"/getflv.txt");
	std::string str;
	stream >> str;
	FormPayload FormPayload;
	FormPayload.readURLEncoded(str);
	ASSERT_FALSE(FormPayload.has(""));
	ASSERT_FALSE(FormPayload.has("__not_having_this__"));
	ASSERT_TRUE(FormPayload.has("thread_id"));
	ASSERT_EQ(1302222473, FormPayload.getLong("thread_id"));
	ASSERT_EQ(99, FormPayload.getLong("l"));
	ASSERT_EQ("http://smile-psu61.nicovideo.jp/smile?m=14097905.36147", FormPayload.getString("url"));
	ASSERT_EQ("http://www.smilevideo.jp/view/14097905/185957", FormPayload.getString("link"));
	ASSERT_EQ("http://msg.nicovideo.jp/36/api/", FormPayload.getString("ms"));
	ASSERT_EQ(185957, FormPayload.getLong("user_id"));
	ASSERT_TRUE(FormPayload.getBool("is_premium"));
	ASSERT_EQ("ψ（プサイ）", FormPayload.getString("nickname"));
	ASSERT_EQ(1332645296599, FormPayload.getLong("time"));
	ASSERT_TRUE(FormPayload.getBool("done"));
	ASSERT_EQ(1302222649, FormPayload.getLong("nicos_id"));
	ASSERT_EQ(70, FormPayload.getLong("ng_rv"));
	ASSERT_EQ("hiroba09.nicovideo.jp", FormPayload.getString("hms"));
	ASSERT_EQ(2529, FormPayload.getLong("hmsp"));
	ASSERT_EQ(1000000165, FormPayload.getLong("hmst"));
	ASSERT_EQ("1332645356.6-xTp4qlgXu2GtTIMPEb4w4ShIw", FormPayload.getString("hmstk"));
	ASSERT_EQ(RPU_JSON, FormPayload.getString("rpu"));
}

}
