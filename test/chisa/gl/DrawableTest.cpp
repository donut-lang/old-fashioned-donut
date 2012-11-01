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
	Handler<Drawable> dr = canv->queryDrawable("color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<ColorDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(100, 100), 1));
	ASSERT_EQ(RED, dr.cast<ColorDrawable>()->color());
}

TEST_F(DrawableTest, ImageDrawableTest)
{
	Handler<Drawable> dr = canv->queryDrawable("image:" MATERIAL_DIR "/img/test.png", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<ImageDrawable*>(dr.get()));
}

TEST_F(DrawableTest, RepeatDrawableTest)
{
	Handler<Drawable> dr = canv->queryDrawable("repeat:color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<RepeatDrawable*>(dr.get()));
}

TEST_F(DrawableTest, StretchDrawableTest)
{
	Handler<Drawable> dr = canv->queryDrawable("stretch:color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<StretchDrawable*>(dr.get()));
}
}}
