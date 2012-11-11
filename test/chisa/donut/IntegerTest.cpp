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
#include "../../../src/chisa/donut/object/World.h"
#include "../../../src/chisa/donut/vm/Machine.h"
#include <math.h>

namespace chisa {
namespace donut {

class DonutIntegerTest : public ::testing::Test
{
protected:
	Handler<Code> code;
	World* world;
public:
	void SetUp(){
		code = Handler<Code>(new Code());
		world = new World(log_trace, code);
	}
	void TearDown(){
		delete world;
		code.reset();
	}
};

TEST_F(DonutIntegerTest, LiteralTest)
{
	unsigned int idx = Parser::fromString("1;", "<MEM>", 0)->parseProgram(code);
	World world(log_trace, code);
	Machine machine(log_trace, &world);

	Handler<Object> result = machine.start(idx);
	ASSERT_TRUE(result->isInt());
	ASSERT_EQ(1, result->toInt(&world));
}

#define SOURCE_TEST(expr, answer)\
	unsigned int idx = Parser::fromString(expr ";", "<MEM>", 0)->parseProgram(code);\
	World world(log_trace, code);\
	Machine machine(log_trace, &world);\
\
	Handler<Object> result = machine.start(idx);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isInt());\
	ASSERT_EQ(answer, result->toInt(&world));

#define SOURCE_TEST_TRUE(expr)\
	unsigned int idx = Parser::fromString(expr ";", "<MEM>", 0)->parseProgram(code);\
	World world(log_trace, code);\
	Machine machine(log_trace, &world);\
\
	Handler<Object> result = machine.start(idx);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_TRUE(result->toBool(&world));

#define SOURCE_TEST_FALSE(expr)\
	unsigned int idx = Parser::fromString(expr ";", "<MEM>", 0)->parseProgram(code);\
	World world(log_trace, code);\
	Machine machine(log_trace, &world);\
\
	Handler<Object> result = machine.start(idx);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_FALSE(result->toBool(&world));

TEST_F(DonutIntegerTest, SubTest)
{
	SOURCE_TEST("1-2", -1);
}

TEST_F(DonutIntegerTest, MulTest)
{
	SOURCE_TEST("3*4", 12);
}

TEST_F(DonutIntegerTest, DivTest)
{
	SOURCE_TEST("9/4", 2);
}

TEST_F(DonutIntegerTest, ModTest)
{
	SOURCE_TEST("9%4", 1);
}

TEST_F(DonutIntegerTest, LtTest)
{
	{
		SOURCE_TEST_TRUE("4<9");
	}
	{
		SOURCE_TEST_FALSE("9<4");
	}
}
TEST_F(DonutIntegerTest, GtTest)
{
	{
		SOURCE_TEST_TRUE("9>4");
	}
	{
		SOURCE_TEST_FALSE("4>9");
	}
}

TEST_F(DonutIntegerTest, LeTest)
{
	{
		SOURCE_TEST_TRUE("4<=9");
	}
	{
		SOURCE_TEST_FALSE("9<=4");
	}
	{
		SOURCE_TEST_TRUE("4<=4");
	}
}
TEST_F(DonutIntegerTest, GeTest)
{
	{
		SOURCE_TEST_TRUE("9>=4");
	}
	{
		SOURCE_TEST_FALSE("4>=9");
	}
	{
		SOURCE_TEST_TRUE("4>=4");
	}
}
TEST_F(DonutIntegerTest, EqTest)
{
	{
		SOURCE_TEST_TRUE("4==4");
	}
	{
		SOURCE_TEST_FALSE("5==4");
	}
}
TEST_F(DonutIntegerTest, NeTest)
{
	{
		SOURCE_TEST_TRUE("5!=4");
	}
	{
		SOURCE_TEST_FALSE("4!=4");
	}
}

TEST_F(DonutIntegerTest, PlusTest)
{
	{
		SOURCE_TEST("+4", 4);
	}
	{
		SOURCE_TEST("+(-4)", -4);
	}
}

TEST_F(DonutIntegerTest, MinusTest)
{
	{
		SOURCE_TEST("-4", -4);
	}
	{
		SOURCE_TEST("-(-4)", 4);
	}
}

}}


