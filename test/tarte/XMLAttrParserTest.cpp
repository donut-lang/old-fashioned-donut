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

#include "../TestCommon.h"
#include <tarte/XMLAttrParser.h>

#include <typeinfo>
#include <cmath>

namespace tarte {

TEST(XMLAttrParser, StringNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	std::string str="TEST";
	xml::parseAttr<std::string>("str", std::ref(str), "TEST2", doc->RootElement());
	ASSERT_EQ("TEST2", str);
}

TEST(XMLAttrParser, StringFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc str="str" float="int" margin="1" />
	)delimiter");
	std::string str="TEST";
	xml::parseAttr<std::string>("str", std::ref(str), "TEST2", doc->RootElement());
	ASSERT_EQ("str", str);
}

#define PARSE(type, first, def) type v = first; xml::parseAttr<type>("val", std::ref(v), def, doc->RootElement());

TEST(XMLAttrParser, FloatNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	PARSE(float, 1, 10);
	ASSERT_FLOAT_EQ(10, v);
}

TEST(XMLAttrParser, FloatFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="1.12" />
	)delimiter");
	PARSE(float, 1, 10);
	ASSERT_FLOAT_EQ(1.12, v);
}

TEST(XMLAttrParser, FloatInvalidTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="string!!" />
	)delimiter");
	PARSE(float, 1, 10);
	ASSERT_FLOAT_EQ(10, v);
}

TEST(XMLAttrParser, FloatNANTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="nan" />
	)delimiter");
	PARSE(float, 1, 10);
	ASSERT_TRUE(std::isnan(v));
}

TEST(XMLAttrParser, IntNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	PARSE(int, 1, 10);
	ASSERT_EQ(10, v);
}

TEST(XMLAttrParser, IntFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="5" />
	)delimiter");
	PARSE(int, 1, 10);
	ASSERT_EQ(5, v);
}

TEST(XMLAttrParser, IntMinusFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="-5" />
	)delimiter");
	PARSE(int, 1, 10);
	ASSERT_EQ(-5, v);
}

TEST(XMLAttrParser, IntInvalidTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="string!!" />
	)delimiter");
	PARSE(int, 1, 10);
	ASSERT_EQ(10, v);
}


TEST(XMLAttrParser, UnsignedIntNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	PARSE(unsigned int, 1, 10);
	ASSERT_EQ(10, v);
}

TEST(XMLAttrParser, UnsignedIntFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="5" />
	)delimiter");
	PARSE(unsigned int, 1, 10);
	ASSERT_EQ(5, v);
}

TEST(XMLAttrParser, UnsignedIntMinusTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="-5" />
	)delimiter");
	PARSE(unsigned int, 1, 10);
	//XXX: …これでいいの？？
	ASSERT_EQ(-5, v);
}

TEST(XMLAttrParser, UnsignedIntInvalidTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="string!!" />
	)delimiter");
	PARSE(unsigned int, 1, 10);
	ASSERT_EQ(10, v);
}

TEST(XMLAttrParser, BoolNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolFoundTrueTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="true" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_TRUE(v);
}

TEST(XMLAttrParser, BoolFoundCapitalTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="True" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolFoundYesTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="yes" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolFoundFalseTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="false" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolFoundNoTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="no" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolNumTrueTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="5" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_TRUE(v);
}

TEST(XMLAttrParser, BoolNumFalseTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="0" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolInvalidTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="string!!" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

}
