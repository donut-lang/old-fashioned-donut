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

#include "../../TestCommon.hpp"
#include "../../../src/chisa/geom/Area.hpp"
#include <cinamo/XMLAttrParser.h>
#include <math.h>

namespace chisa {
namespace geom {

#define TEST_GETTER_SETTER(var, name, value)\
	var.name(value);\
	ASSERT_FLOAT_EQ(var.name(), value);

TEST(SpaceTest, BasicTest)
{
	Space m;
	TEST_GETTER_SETTER(m, top, 1);
	TEST_GETTER_SETTER(m, bottom, 2);
	TEST_GETTER_SETTER(m, right, 3);
	TEST_GETTER_SETTER(m, left, 4);
}

TEST(SpaceTest, ConstructorTest)
{
	Space m(1,2,10,20);
	ASSERT_FLOAT_EQ(1,m.top());
	ASSERT_FLOAT_EQ(2,m.bottom());
	ASSERT_FLOAT_EQ(10,m.left());
	ASSERT_FLOAT_EQ(20,m.right());
}

TEST(SpaceTest, OffsetTest)
{
	Space m(1,2,10,20);
	ASSERT_FLOAT_EQ(10, m.offset().x());
	ASSERT_FLOAT_EQ(1, m.offset().y());
}

TEST(SpaceTest, TotalSpaceTest)
{
	Space m(1,2,10,20);
	ASSERT_FLOAT_EQ(10+20, m.totalSpace().x());
	ASSERT_FLOAT_EQ(10+20, m.totalWidth());
	ASSERT_FLOAT_EQ(1+2, m.totalSpace().y());
	ASSERT_FLOAT_EQ(1+2, m.totalHeight());
}

TEST(SpaceTest, XMLParseNotFoundTest)
{
	Space m(1,2,10,20);
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc/>
	)delimiter");
	xml::parseAttr<Space>("margin", std::ref(m), std::ref(m), doc->RootElement());
	ASSERT_FLOAT_EQ(1,m.top());
	ASSERT_FLOAT_EQ(2,m.bottom());
	ASSERT_FLOAT_EQ(10,m.left());
	ASSERT_FLOAT_EQ(20,m.right());
}

TEST(SpaceTest, XMLParseInvalidTest)
{
	Space m(1,2,10,20);
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc margin="string!!" />
	)delimiter");
	xml::parseAttr<Space>("margin", std::ref(m), std::ref(m), doc->RootElement());
	ASSERT_FLOAT_EQ(1,m.top());
	ASSERT_FLOAT_EQ(2,m.bottom());
	ASSERT_FLOAT_EQ(10,m.left());
	ASSERT_FLOAT_EQ(20,m.right());
}

TEST(SpaceTest, XMLParseOnlySpaceTest)
{
	Space m(1,2,10,20);
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc margin="100" />
	)delimiter");
	xml::parseAttr<Space>("margin", std::ref(m), std::ref(m), doc->RootElement());
	ASSERT_FLOAT_EQ(100,m.top());
	ASSERT_FLOAT_EQ(100,m.bottom());
	ASSERT_FLOAT_EQ(100,m.left());
	ASSERT_FLOAT_EQ(100,m.right());
}

TEST(SpaceTest, XMLParseAllSpaceTest)
{
	Space m(1,2,10,20);
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc margin-top="100" margin-bottom="200" margin-left="1000" margin-right="2000" />
	)delimiter");
	xml::parseAttr<Space>("margin", std::ref(m), std::ref(m), doc->RootElement());
	ASSERT_FLOAT_EQ(100,m.top());
	ASSERT_FLOAT_EQ(200,m.bottom());
	ASSERT_FLOAT_EQ(1000,m.left());
	ASSERT_FLOAT_EQ(2000,m.right());
}

TEST(SpaceTest, XMLParseAllMarginOverrideTest)
{
	Space m(1,2,10,20);
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc margin="0" margin-top="100" margin-bottom="200" margin-left="1000" margin-right="2000" />
	)delimiter");
	xml::parseAttr<Space>("margin", std::ref(m), std::ref(m), doc->RootElement());
	ASSERT_FLOAT_EQ(100,m.top());
	ASSERT_FLOAT_EQ(200,m.bottom());
	ASSERT_FLOAT_EQ(1000,m.left());
	ASSERT_FLOAT_EQ(2000,m.right());
}

}}
