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
#include <math.h>
#include "../../../src/chisa/gl/DrawableManager.hpp"
#include "../../../src/chisa/gl/Canvas.hpp"

namespace chisa {
namespace gl {

class FontTest : public ::testing::Test
{
protected:
	Canvas* cv;
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


TEST_F(FontTest, AnalyzeTest)
{
	std::string family="a",style="b";
	gl::Font::analyzeFontName("Family:Style", family, style);
	ASSERT_EQ("Family", family);
	ASSERT_EQ("Style", style);

	family="a",style="b";
	gl::Font::analyzeFontName("Family", family, style);
	ASSERT_EQ("Family", family);
	ASSERT_EQ("", style);

	family="a",style="b";
	gl::Font::analyzeFontName("Family:", family, style);
	ASSERT_EQ("Family", family);
	ASSERT_EQ("", style);


	family="a",style="b";
	gl::Font::analyzeFontName("Family:Style::::::aa:a:a:a:", family, style);
	ASSERT_EQ("Family", family);
	ASSERT_EQ("Style", style);
}

}}
