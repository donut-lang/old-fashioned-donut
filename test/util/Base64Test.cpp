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
#include <tarte/Base64.h>

#include <cfloat>
#include <cmath>

namespace tarte {

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
	ASSERT_EQ("test", base64::decodeAsString("dGVzdA=="));

	ASSERT_EQ("あいうえお", base64::decodeAsString("44GC44GE44GG44GI44GK"));
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
