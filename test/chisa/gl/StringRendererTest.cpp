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
#include "../../../src/chisa/gl/StringRenderer.h"
#include <math.h>

namespace chisa {
namespace gl {

TEST(StringRendererTest, BasicTest)
{
	StringRenderer r;
	StringRenderer::Command aa = r.measure("a");
	StringRenderer::Command ab = r.measure("あ");
	StringRenderer::Command cmd = r.calcMaximumStringLength("aあc", aa.area().width()+ab.area().width()+0.0001);
	ASSERT_EQ("aあ", cmd.str());
	ASSERT_FLOAT_EQ(aa.area().width()+ab.area().width(), cmd.area().width());

	cmd = r.calcMaximumStringLength("aあc", aa.area().width()+ab.area().width()-0.0001);
	ASSERT_EQ("a", cmd.str());
	ASSERT_FLOAT_EQ(aa.area().width(), cmd.area().width());
}

TEST(StringRendererTest, EmptySpaceTest)
{

	StringRenderer r;
	StringRenderer::Command cmd = r.calcMaximumStringLength("aあc", r.measure("a").area().width()-0.0001);
	ASSERT_EQ("", cmd.str());
	ASSERT_FLOAT_EQ(0, cmd.area().width());
}

}}

