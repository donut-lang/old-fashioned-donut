/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Base64.h>

#include <cfloat>
#include <cmath>

namespace cinamo {

TEST(StringUtilTest, Base64EecodeTest)
{
	{
		std::vector<char> d;
		d.push_back('t');
		d.push_back('e');
		d.push_back('s');
		d.push_back('t');
		ASSERT_EQ("dGVzdA==", base64::encode(d));
	}
	ASSERT_EQ("dGVzdA==", base64::encode("test"));
	ASSERT_EQ("dGVzdA==", base64::encode("test", 4));

	ASSERT_EQ("dA==", base64::encode("t"));
	ASSERT_EQ("dGU=", base64::encode("te"));
	ASSERT_EQ("dGVz", base64::encode("tes"));
	ASSERT_EQ("dGVzdA==", base64::encode("test"));

	ASSERT_EQ("44GC44GE44GG44GI44GK", base64::encode("あいうえお"));
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
		ASSERT_EQ(d, base64::decode("dABzdAB0FQD/Kg=="));
	}
}

TEST(StringUtilTest, Base64DecodeTest)
{
	std::vector<char> d;
	d.push_back('t');
	d.push_back('e');
	d.push_back('s');
	d.push_back('t');
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+1), base64::decode("dA=="));
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+2), base64::decode("dGU="));
	ASSERT_EQ(std::vector<char>(d.begin(), d.begin()+3), base64::decode("dGVz"));
	ASSERT_EQ(d, base64::decode("dGVzdA=="));
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
		ASSERT_EQ("dABzdAB0FQD/Kg==", base64::encode(d));
		ASSERT_EQ(d, base64::decode(base64::encode(d)));
	}
	ASSERT_EQ("test", base64::decode<std::string>("dGVzdA=="));

	ASSERT_EQ("あいうえお", base64::decode<std::string>("44GC44GE44GG44GI44GK"));
}

TEST(StringUtilTest, Base64Test)
{
	std::vector<char> a;
	std::vector<char> b;
	{
		for(int i=0;i<256;++i){
			a.push_back(i);
			b.push_back(i);
		}
		std::string s ( base64::encode(b) );
		b.clear();
		b = base64::decode( s );
	}
	ASSERT_EQ(a, b);
}

}
