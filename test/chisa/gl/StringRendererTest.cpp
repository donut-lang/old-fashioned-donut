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
#include <math.h>

namespace chisa {
namespace doc {

/*
class StringRendererTest : public ::testing::Test
{
protected:
	StringRenderer* r;
public:
	void SetUp(){
		r = new StringRenderer(Handler<gl::FontManager>(new FontManager(log_trace, MATERIAL_DIR"/font")));
	}
	void TearDown(){
		delete r;
		r = nullptr;
	}
};


TEST_F(StringRendererTest, BasicTest)
{
	StringRenderer::Command aa = r->measure("a");
	StringRenderer::Command ab = r->measure("あ");
	StringRenderer::Command cmd = r->calcMaximumStringLength("aあc", aa.width()+ab.width()+0.0001);
	ASSERT_EQ("aあ", cmd.str());
	ASSERT_FLOAT_EQ(aa.width()+ab.width(), cmd.width());

	cmd = r->calcMaximumStringLength("aあc", aa.width()+ab.width()-0.0001);
	ASSERT_EQ("a", cmd.str());
	ASSERT_FLOAT_EQ(aa.width(), cmd.width());
}

TEST_F(StringRendererTest, EmptySpaceTest)
{

	StringRenderer::Command cmd = r->calcMaximumStringLength("aあc", r->measure("a").width()-0.0001);
	ASSERT_EQ("", cmd.str());
	ASSERT_FLOAT_EQ(0, cmd.width());
}

TEST_F(StringRendererTest, StylePushTest)
{
	r->pushStyle(StringRenderer::Style::Bold);
	ASSERT_EQ(StringRenderer::Style::Bold, r->nowStyle());
	r->pushStyle(StringRenderer::Style::Italic);
	ASSERT_EQ(StringRenderer::Style::Italic, r->nowStyle());
	r->popStyle();
	ASSERT_EQ(StringRenderer::Style::Bold, r->nowStyle());
}

TEST_F(StringRendererTest, SizePushTest)
{
	r->pushSize(12.345);
	ASSERT_FLOAT_EQ(12.345, r->nowSize());
	r->pushSize(45.678);
	ASSERT_FLOAT_EQ(45.678, r->nowSize());
	r->popSize();
	ASSERT_FLOAT_EQ(12.345, r->nowSize());
}

TEST_F(StringRendererTest, ColorPushTest)
{
	r->pushColor(RED);
	ASSERT_EQ(RED, r->nowColor());
	r->pushColor(BLUE);
	ASSERT_EQ(BLUE, r->nowColor());
	r->popColor();
	ASSERT_EQ(RED, r->nowColor());
}

TEST_F(StringRendererTest, FontPushTest)
{
	r->pushFont("VL Gothic:regular");
	ASSERT_EQ("VL Gothic:regular", r->nowFont());
	r->pushFont("VL Gothic:bold");
	ASSERT_EQ("VL Gothic:bold", r->nowFont());
	r->popFont();
	ASSERT_EQ("VL Gothic:regular", r->nowFont());
}
*/

}}

