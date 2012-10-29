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
#include "../../../src/chisa/gl/Color.h"
#include "../../../src/chisa/util/XMLUtil.h"

namespace chisa {
namespace gl {

#define TEST_GETTER_SETTER(color, name, val)\
	color.name(val);\
	ASSERT_FLOAT_EQ(val, color.name());\
	ASSERT_FALSE(c.isUnchangedColor());

TEST(ColorTest, GetSetTest)
{
	Color c;
	TEST_GETTER_SETTER(c, red, 0.1);
	TEST_GETTER_SETTER(c, blue, 0.4);
	TEST_GETTER_SETTER(c, green, 0.21);
	TEST_GETTER_SETTER(c, alpha, 0.4);
}

TEST(ColorTest, AlphaMultiplyTest)
{
	Color c(1,1,1,1);
	ASSERT_FALSE(c.isUnchangedColor());
	Color const c1 = c.multAlpha(0.2f);
	ASSERT_FLOAT_EQ(0.2, c1.alpha());
	ASSERT_FALSE(c1.isUnchangedColor());

	Color const c2 = c1.multAlpha(0.7f);
	ASSERT_FLOAT_EQ(0.2*0.7, c2.alpha());
	ASSERT_FALSE(c2.isUnchangedColor());
}

TEST(ColorTest, EqualityTest)
{
	ASSERT_TRUE(RED != BLUE);
	ASSERT_FALSE(RED == BLUE);
	ASSERT_TRUE(RED == RED);
	ASSERT_FALSE(RED != RED);
}

TEST(ColorTest, UnchangedColorTest)
{
	ASSERT_TRUE(UNCHANGED_COLOR.isUnchangedColor());
	ASSERT_FALSE(RED.isUnchangedColor());
}

TEST(ColorTest, StringParsingTest)
{
	using namespace chisa::util::xml;
	using namespace tinyxml2;
	XMLDocument doc;
	XMLElement* elm = doc.NewElement("a");
	Color c;
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_TRUE(c.isUnchangedColor());

	elm->SetAttribute("color", "fff");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_TRUE(c.isUnchangedColor());

	elm->SetAttribute("color", "#fff");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_FALSE(c.isUnchangedColor());
	ASSERT_FLOAT_EQ(1, c.alpha());
	ASSERT_FLOAT_EQ(1, c.red());
	ASSERT_FLOAT_EQ(1, c.green());
	ASSERT_FLOAT_EQ(1, c.blue());

	elm->SetAttribute("color", "#8888");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	float const middle = 0x88/255.0f;
	ASSERT_FLOAT_EQ(middle, c.alpha());
	ASSERT_FLOAT_EQ(middle, c.red());
	ASSERT_FLOAT_EQ(middle, c.green());
	ASSERT_FLOAT_EQ(middle, c.blue());

	elm->SetAttribute("color", "#ffffff");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_FLOAT_EQ(1.0f, c.alpha());
	ASSERT_FLOAT_EQ(1.0f, c.red());
	ASSERT_FLOAT_EQ(1.0f, c.green());
	ASSERT_FLOAT_EQ(1.0f, c.blue());

	elm->SetAttribute("color", "#80808080");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	float const middle2 = 0x80/255.0f;
	ASSERT_FLOAT_EQ(middle2, c.alpha());
	ASSERT_FLOAT_EQ(middle2, c.red());
	ASSERT_FLOAT_EQ(middle2, c.green());
	ASSERT_FLOAT_EQ(middle2, c.blue());

	elm->SetAttribute("color", "#8080808021212");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_TRUE(c.isUnchangedColor());

	elm->SetAttribute("color", "#80808080");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);

	elm->SetAttribute("color", "#yyyyyyyy");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_TRUE(c.isUnchangedColor());
}

TEST(ColorTest, StringNameParsingTest)
{
	using namespace chisa::util::xml;
	using namespace tinyxml2;
	XMLDocument doc;
	Color c;
	XMLElement* elm = doc.NewElement("a");
	elm->SetAttribute("color", "red");
	parseAttr<Color>("color", c, UNCHANGED_COLOR, elm);
	ASSERT_FALSE(c.isUnchangedColor());

	ASSERT_FLOAT_EQ(1.0f, c.alpha());
	ASSERT_FLOAT_EQ(1.0f, c.red());
	ASSERT_FLOAT_EQ(0.0f, c.green());
	ASSERT_FLOAT_EQ(0.0f, c.blue());
}

}}

