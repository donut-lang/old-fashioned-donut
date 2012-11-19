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
#include "../../../src/chisa/donut/parser/Parser.h"
#include <math.h>

namespace chisa {
namespace donut {

class DonutParserTest : public ::testing::Test
{
protected:
	Handler<Source> code;
public:
	void SetUp(){
		code = Handler<Source>(new Source());
	}
	void TearDown(){
		code.reset();
	}
};
TEST_F(DonutParserTest, BasicTest)
{
	ASSERT_NO_THROW( Parser::fromString("test=1;", "<MEM>", 0)->parseProgram(code) );
}

TEST_F(DonutParserTest, BasicCheckTest)
{
	unsigned int const idx = Parser::fromString("test=1;", "<MEM>", 0)->parseProgram(code);
	ASSERT_EQ(1, code->numClosure());
	Handler<Closure> clos = code->getClosure(idx);

	ASSERT_EQ(0, clos->arglist().size());
	ASSERT_LE(0, clos->instlist().size());
}

TEST_F(DonutParserTest, DotTest)
{
	unsigned int const idx = Parser::fromString("func(zux){test.aux=1;};", "<MEM>", 0)->parseProgram(code);
	ASSERT_EQ(2, code->numClosure());
	Handler<Closure> clos = code->getClosure(idx);

	ASSERT_EQ(0, clos->arglist().size());
	ASSERT_LT(0, clos->instlist().size());

	Handler<Closure> another = code->getClosure((idx+1)%2);
	ASSERT_EQ(1, another->arglist().size());
	ASSERT_LT(0, another->instlist().size());
}

TEST_F(DonutParserTest, BinaryOpTest)
{
	unsigned int const idx = Parser::fromString(R"delimiter(
test1=2;
test2+=1;
test3-=5;
test4*=5.2;
test5/=5;
test6%=5;
test7=test*=2;
test8=test+1;
test9=test-1;
test10=test*1;
test11=test/1;
test12=test%1;
			)delimiter", "<MEM>", 0)->parseProgram(code);
	ASSERT_EQ(1, code->numClosure());
	Handler<Closure> clos = code->getClosure(idx);

	ASSERT_EQ(0, clos->arglist().size());
	ASSERT_LE(0, clos->instlist().size());
}

TEST_F(DonutParserTest, ArrayLiteralTest)
{
	unsigned int const idx = Parser::fromString(R"delimiter(
[10,20,30,"test",12.3,];
[10,20,30,"test",12.3];
[];
			)delimiter", "<MEM>", 0)->parseProgram(code);
	ASSERT_EQ(1, code->numClosure());
	Handler<Closure> clos = code->getClosure(idx);

	ASSERT_EQ(0, clos->arglist().size());
	ASSERT_LE(0, clos->instlist().size());
}

TEST_F(DonutParserTest, ObjectLiteralTest)
{
	unsigned int const idx = Parser::fromString(R"delimiter(
{a=>"a1", b=>"b2", c=>"c3",}; //カンマあり
{a=>"a1", b=>"b2", c=>"c3"}; //カンマなし
{}; //空
	)delimiter", "<MEM>", 0)->parseProgram(code);
	ASSERT_EQ(1, code->numClosure());
	Handler<Closure> clos = code->getClosure(idx);

	ASSERT_EQ(0, clos->arglist().size());
	ASSERT_LE(0, clos->instlist().size());
}

}}


