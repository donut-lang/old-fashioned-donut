/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Url.h>

namespace cinamo {
namespace url {

TEST(UrlTest, PercentDecodeTest)
{
	ASSERT_EQ("NOT_ENCODED", decodePercent("NOT_ENCODED"));
	ASSERT_EQ("test test", decodePercent("test%20test"));
	ASSERT_EQ("test PERCENT_ENCOD\r\nED", decodePercent("test%20%50%45%52%43%45%4e%54%5f%45%4e%43%4f%44%0d%0a%45%44"));
	ASSERT_EQ("test+test", decodePercent("test+test"));
	ASSERT_EQ("test++", decodePercent("test++"));
	ASSERT_EQ("\x20", decodePercent("%20"));
	ASSERT_EQ("\xff", decodePercent("%ff"));
}

TEST(UrlTest, PercentEncodeTest)
{
	ASSERT_EQ("NOT_ENCODED", encodePercent("NOT_ENCODED"));
	ASSERT_EQ("test%20test", encodePercent("test test"));
	ASSERT_EQ("test%20PERCENT_ENCOD%0D%0AED", encodePercent("test PERCENT_ENCOD\r\nED"));
	ASSERT_EQ("test+test", encodePercent("test+test"));
	ASSERT_EQ("%20", encodePercent("\x20"));
	ASSERT_EQ("%FF", encodePercent("\xff"));

	ASSERT_EQ("%E3%82%A6%E3%82%A3%E3%82%AD%E3%83%9A%E3%83%87%E3%82%A3%E3%82%A2", encodePercent("ウィキペディア"));
}

#define BEFORE (R"delimiter(thread_id=1302222473&l=99&url=http://smile-psu61.nicovideo.jp/smile?m=14097905.36147&link=http://www.smilevideo.jp/view/14097905/185957&ms=http://msg.nicovideo.jp/36/api/&user_id=185957&is_premium=1&nickname=ψ（プサイ）&time=1332645296599&done=true&nicos_id=1302222649&ng_rv=70&ng_up=test=tt&%2Atest2=tt&hms=hiroba09.nicovideo.jp&hmsp=2529&hmst=1000000165&hmstk=1332645356.6-xTp4qlgXu2GtTIMPEb4w4ShIw&rpu={"count":1595356,"users":["\u3061\u3073\u7f8e\u3002","\u308c\u3093\u3075\u3041","harutti92","247 \/ \u4ec1\u79d1","nikola","\u4e9c\u9f8d","rc42","\u307e\u3042","\u3081\u3081","shojikatyou","\u5929\u6d25\u7518\u6817","\u30a2\u30e9\u30b8\u30f3\u30ca\u30a4\u30c8","\u3082\u3081\u3093","sleep714","\u30e9\u30de","\u3082\u3063\u3061\u3083\u3093","\u30ca\u30eb\u30b9","Ryousuke","ikaros","\u306f\u308b\u3054\u3093\u592a\u90ce"],"extra":18})delimiter")
#define AFTER ("thread_id=1302222473&l=99&url=http%3A%2F%2Fsmile-psu61.nicovideo.jp%2Fsmile%3Fm%3D14097905.36147&link=http%3A%2F%2Fwww.smilevideo.jp%2Fview%2F14097905%2F185957&ms=http%3A%2F%2Fmsg.nicovideo.jp%2F36%2Fapi%2F&user_id=185957&is_premium=1&nickname=%CF%88%EF%BC%88%E3%83%97%E3%82%B5%E3%82%A4%EF%BC%89&time=1332645296599&done=true&nicos_id=1302222649&ng_rv=70&ng_up=test%3Dtt%26%252Atest2%3Dtt&hms=hiroba09.nicovideo.jp&hmsp=2529&hmst=1000000165&hmstk=1332645356.6-xTp4qlgXu2GtTIMPEb4w4ShIw&rpu=%7B%22count%22%3A1595356%2C%22users%22%3A%5B%22%5Cu3061%5Cu3073%5Cu7f8e%5Cu3002%22%2C%22%5Cu308c%5Cu3093%5Cu3075%5Cu3041%22%2C%22harutti92%22%2C%22247+%5C%2F+%5Cu4ec1%5Cu79d1%22%2C%22nikola%22%2C%22%5Cu4e9c%5Cu9f8d%22%2C%22rc42%22%2C%22%5Cu307e%5Cu3042%22%2C%22%5Cu3081%5Cu3081%22%2C%22shojikatyou%22%2C%22%5Cu5929%5Cu6d25%5Cu7518%5Cu6817%22%2C%22%5Cu30a2%5Cu30e9%5Cu30b8%5Cu30f3%5Cu30ca%5Cu30a4%5Cu30c8%22%2C%22%5Cu3082%5Cu3081%5Cu3093%22%2C%22sleep714%22%2C%22%5Cu30e9%5Cu30de%22%2C%22%5Cu3082%5Cu3063%5Cu3061%5Cu3083%5Cu3093%22%2C%22%5Cu30ca%5Cu30eb%5Cu30b9%22%2C%22Ryousuke%22%2C%22ikaros%22%2C%22%5Cu306f%5Cu308b%5Cu3054%5Cu3093%5Cu592a%5Cu90ce%22%5D%2C%22extra%22%3A18%7D")
TEST(UrlTest, FormDecodeTest)
{
	ASSERT_EQ(BEFORE, decodeForm(AFTER));
}

TEST(UrlTest, FormEncodeTest)
{
	ASSERT_EQ("http%3A%2F%2Fsmile-psu61.nicovideo.jp%2Fsmile%3Fm%3D14097905.36147", encodeForm("http://smile-psu61.nicovideo.jp/smile?m=14097905.36147"));
}

}}
