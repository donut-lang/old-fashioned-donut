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

#include "Helper.h"
#include <math.h>

namespace chisa {
namespace donut {

class SourceSerializeTest : public ::testing::Test
{
public:
	void SetUp(){
	}
	void TearDown(){
	}
};

TEST_F(SourceSerializeTest, SourceSerializeTest)
{
	Handler<Source> code1 = Parser::fromString(R"delimiter(
{a=>"a1", b=>"b2", c=>"c3",}; //カンマあり
{a=>"a1", b=>"b2", c=>"c3"}; //カンマなし
{}; //空
f = func(x,y,z){ x+y+z; };
	)delimiter", "<MEM>", 0)->parseProgram();
	Handler<Closure> clos = code1->getEntrypoint();

	util::XValue val = code1->save();
	Handler<Source> code2 (new Source(val));
	ASSERT_NE(code1, code2);
	ASSERT_TRUE(*code1 == *code2);
}

TEST_F(SourceSerializeTest, ClosureSaveTest)
{
	Handler<Source> code = Parser::fromString(R"delimiter(
{a=>"a1", b=>"b2", c=>"c3",}; //カンマあり
{a=>"a1", b=>"b2", c=>"c3"}; //カンマなし
{}; //空
f = func(x,y,z){ x+y+z; };
	)delimiter", "<MEM>", 0)->parseProgram();
	Handler<Closure> clos = code->getEntrypoint();

	util::XValue val = clos->save();
	Handler<Closure> clos2 (new Closure(val));
	ASSERT_NE(clos, clos2);
	ASSERT_EQ(*clos, *clos2);
}

}}


