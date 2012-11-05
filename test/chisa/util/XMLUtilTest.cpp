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

#include "../../TestCommon.h"
#include "../../src/chisa/util/XMLUtil.h"

#include <typeinfo>
#include <cmath>

namespace chisa {
namespace util {

TEST(XMLUtilTest, StringNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	std::string str="TEST";
	util::xml::parseAttr<std::string>("str", std::ref(str), "TEST2", doc->RootElement());
	ASSERT_EQ("TEST2", str);
}

TEST(XMLUtilTest, StringFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc str="str" float="int" margin="1" />
	)delimiter");
	std::string str="TEST";
	util::xml::parseAttr<std::string>("str", std::ref(str), "TEST2", doc->RootElement());
	ASSERT_EQ("str", str);
}

TEST(XMLUtilTest, FloatNotFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc />
	)delimiter");
	float num = 1;
	util::xml::parseAttr<float>("str", std::ref(num), 10, doc->RootElement());
	ASSERT_FLOAT_EQ(10, num);
}

TEST(XMLUtilTest, FloatFoundTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc float="1.12" />
	)delimiter");
	float num = 1;
	util::xml::parseAttr<float>("float", std::ref(num), 10, doc->RootElement());
	ASSERT_FLOAT_EQ(1.12, num);
}

TEST(XMLUtilTest, FloatInvalidTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc float="string!!" />
	)delimiter");
	float num = 1;
	util::xml::parseAttr<float>("float", std::ref(num), 10, doc->RootElement());
	ASSERT_FLOAT_EQ(10, num);
}

TEST(XMLUtilTest, FloatNANTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?><doc float="nan" />
	)delimiter");
	float num = 1;
	util::xml::parseAttr<float>("float", std::ref(num), 10, doc->RootElement());
	ASSERT_TRUE(std::isnan(num));
}

}}
