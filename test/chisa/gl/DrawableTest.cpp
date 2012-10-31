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
#include "../../../src/chisa/gl/Canvas.h"
#include "../../../src/chisa/gl/Drawable.h"

namespace chisa {
namespace gl {

class DrawableTest : public ::testing::Test
{
protected:
	Canvas* canv;
public:
	void SetUp(){
		canv = new Canvas(log_trace);
	}
	void TearDown(){
		delete canv;
		canv = nullptr;
	}
};

TEST_F(DrawableTest, ColorDrawableTest)
{
	ColorDrawable* dr = new ColorDrawable(RED, geom::Box(100, 100));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(100, 100), 1));
	ASSERT_EQ(RED, dr->color());
	delete dr;
}

TEST_F(DrawableTest, SpriteDrawableTest)
{
	Handler<gl::RawSprite> spr = canv->queryImage(MATERIAL_DIR"/img/test.png");
	SpriteDrawable* dr = new SpriteDrawable(spr);
	ASSERT_FLOAT_EQ(spr->width(), dr->width());
	ASSERT_FLOAT_EQ(spr->height(), dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(spr->width(), spr->height()), 1));
	ASSERT_EQ(spr, dr->sprite());
	delete dr;
}

TEST_F(DrawableTest, RepeatDrawableTest)
{
	Handler<ColorDrawable> cdr(new ColorDrawable(RED, geom::Box(100, 100)));
	RepeatDrawable* dr = new RepeatDrawable(cdr, geom::Box(200, 200));
	ASSERT_EQ(cdr, dr->child());
	delete dr;
}

TEST_F(DrawableTest, StretchDrawableTest)
{
	Handler<ColorDrawable> cdr(new ColorDrawable(RED, geom::Box(100, 100)));
	StretchDrawable* dr = new StretchDrawable(cdr, geom::Box(200, 200));
	delete dr;
}
}}
