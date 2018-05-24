/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/XMLAttrParser.h>

#include <typeinfo>
#include <cmath>

namespace cinamo {

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
	ASSERT_TRUE(v);
}

TEST(XMLAttrParser, BoolFoundYesTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="yes" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_TRUE(v);
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

TEST(XMLAttrParser, BoolNumNotZeroTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc val="5" />
	)delimiter");
	PARSE(bool, true, false);
	ASSERT_FALSE(v);
}

TEST(XMLAttrParser, BoolNumZeroTest)
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
