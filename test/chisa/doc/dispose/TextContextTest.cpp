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

#include "../../../TestCommon.h"
#include "../../src/chisa/gl/DrawableManager.h"
#include "../../src/chisa/gl/Color.h"
#include "../../src/chisa/doc/render/RenderTree.h"
#include "../../src/chisa/doc/dispose/TextContext.h"
#include <math.h>

namespace chisa {
namespace doc {

class TextContextTest : public ::testing::Test
{
protected:
	Handler<gl::DrawableManager> mgr;
	Handler<RenderTree> tree;
	TextContext* r;
public:
	void SetUp(){
		mgr = Handler<gl::DrawableManager>(new gl::DrawableManager(log_trace, gl::DrawableSetting(MATERIAL_DIR"/font")));
		tree = Handler<RenderTree>(new RenderTree(log_trace, mgr));
		r = new TextContext(log_trace, tree);
	}
	void TearDown(){
		delete r;
		tree.reset();
		mgr.reset();
	}
};


TEST_F(TextContextTest, BasicTest)
{
	geom::Box aa = r->measure("a");
	ASSERT_LE(0, aa.width());
	ASSERT_LE(0, aa.height());
	Handler<TextDrawableObject> cmd = r->create("a", HandlerW<Node>(), 1);
	ASSERT_FLOAT_EQ(aa.width(), cmd->width());
	ASSERT_FLOAT_EQ(aa.height(), cmd->height());
}

TEST_F(TextContextTest, EmptySpaceTest)
{

	geom::Box aa = r->measure("a");
	geom::Box ab = r->measure("b");
	Handler<TextDrawableObject> cmd = r->create("ab", HandlerW<Node>(), 1);
	ASSERT_FLOAT_EQ(aa.width()+ab.width(), cmd->width());
	ASSERT_FLOAT_EQ(geom::max(aa.height(), ab.height()), cmd->height());
}

TEST_F(TextContextTest, StylePushTest)
{
	r->pushStyle(gl::TextDrawable::Style::Bold);
	ASSERT_EQ(gl::TextDrawable::Style::Bold, r->nowStyle());
	r->pushStyle(gl::TextDrawable::Style::Italic);
	ASSERT_EQ(gl::TextDrawable::Style::Italic, r->nowStyle());
	r->popStyle();
	ASSERT_EQ(gl::TextDrawable::Style::Bold, r->nowStyle());
}

TEST_F(TextContextTest, SizePushTest)
{
	r->pushSize(12.345);
	ASSERT_FLOAT_EQ(12.345, r->nowSize());
	r->pushSize(45.678);
	ASSERT_FLOAT_EQ(45.678, r->nowSize());
	r->popSize();
	ASSERT_FLOAT_EQ(12.345, r->nowSize());
}

TEST_F(TextContextTest, ColorPushTest)
{
	r->pushColor(gl::RED);
	ASSERT_EQ(gl::RED, r->nowColor());
	r->pushColor(gl::BLUE);
	ASSERT_EQ(gl::BLUE, r->nowColor());
	r->popColor();
	ASSERT_EQ(gl::RED, r->nowColor());
}

TEST_F(TextContextTest, BackColorPushTest)
{
	r->pushBackColor(gl::RED);
	ASSERT_EQ(gl::RED, r->nowBackColor());
	r->pushBackColor(gl::BLUE);
	ASSERT_EQ(gl::BLUE, r->nowBackColor());
	r->popBackColor();
	ASSERT_EQ(gl::RED, r->nowBackColor());
}
TEST_F(TextContextTest, FontPushTest)
{
	r->pushFont("VL Gothic:regular");
	ASSERT_EQ("VL Gothic:regular", r->nowFont());
	r->pushFont("VL Gothic:bold");
	ASSERT_EQ("VL Gothic:bold", r->nowFont());
	r->popFont();
	ASSERT_EQ("VL Gothic:regular", r->nowFont());
}

}}

