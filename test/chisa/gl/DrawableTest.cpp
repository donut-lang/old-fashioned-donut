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
#include "../../../src/chisa/gl/DrawableManager.hpp"
#include "../../../src/chisa/gl/Canvas.hpp"

namespace chisa {
namespace gl {

class DrawableTest : public ::testing::Test
{
protected:
	Canvas *cv;
	Handler<DrawableManager> dmanager;
public:
	void SetUp(){
		cv = new Canvas(log_trace);
		dmanager = Handler<DrawableManager>(new DrawableManager(log_trace, *cv, DrawableSetting(MATERIAL_DIR"/font")));
	}
	void TearDown(){
		dmanager.reset();
		delete cv;
	}
};
TEST_F(DrawableTest, ColorDrawableTest)
{
	Handler<Drawable> dr = dmanager->queryDrawable("color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<ColorDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(100, 100), 1));
	ASSERT_EQ(Red, dr.cast<ColorDrawable>()->color());
}

TEST_F(DrawableTest, ImageDrawableWithSizeTest)
{
	Handler<Drawable> dr = dmanager->queryDrawable("image:" MATERIAL_DIR "/img/test.png", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<ImageDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(100, 100), 1));
	Handler<ImageDrawable> img(dr.cast<ImageDrawable>());
	ASSERT_EQ(360, img->sprite()->width());
	ASSERT_EQ(480, img->sprite()->height());
}

TEST_F(DrawableTest, ImageDrawableWithoutSizeTest)
{
	Handler<Drawable> dr = dmanager->queryDrawable("image:" MATERIAL_DIR "/img/test.png", geom::Box());
	ASSERT_TRUE(dynamic_cast<ImageDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(360, dr->width());
	ASSERT_FLOAT_EQ(480, dr->height());
	ASSERT_TRUE(dr->size().near(geom::Box(360, 480), 1));
	Handler<ImageDrawable> img(dr.cast<ImageDrawable>());
	ASSERT_EQ(360, img->sprite()->width());
	ASSERT_EQ(480, img->sprite()->height());
}

TEST_F(DrawableTest, RepeatDrawableTest)
{
	Handler<Drawable> dr = dmanager->queryDrawable("repeat:color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<RepeatDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
}

TEST_F(DrawableTest, StretchDrawableTest)
{
	Handler<Drawable> dr = dmanager->queryDrawable("stretch:color:red", geom::Box(100,100));
	ASSERT_TRUE(dynamic_cast<StretchDrawable*>(dr.get()));
	ASSERT_FLOAT_EQ(100, dr->width());
	ASSERT_FLOAT_EQ(100, dr->height());
}

}}
