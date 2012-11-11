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

class DonutBooleanTest : public ::testing::Test
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


#define SOURCE_TEST_TRUE(expr)\
{\
	unsigned int idx = Parser::fromString(expr ";", "<MEM>", 0)->parseProgram(code);\
	World world(log_trace, code);\
	Machine machine(log_trace, &world);\
\
	Handler<Object> result = machine.start(idx);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_TRUE(result->toBool(&world));\
}

#define SOURCE_TEST_FALSE(expr)\
{\
	unsigned int idx = Parser::fromString(expr ";", "<MEM>", 0)->parseProgram(code);\
	World world(log_trace, code);\
	Machine machine(log_trace, &world);\
\
	Handler<Object> result = machine.start(idx);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_FALSE(result->toBool(&world));\
}

TEST_F(DonutBooleanTest, LiteralTest)
{
	SOURCE_TEST_TRUE("true");
	SOURCE_TEST_FALSE("false");
}
TEST_F(DonutBooleanTest, AndTest)
{
	SOURCE_TEST_FALSE("false && false");
	SOURCE_TEST_FALSE("false && true");
	SOURCE_TEST_FALSE("true && false");
	SOURCE_TEST_TRUE("true && true");
}
TEST_F(DonutBooleanTest, OrTest)
{
	SOURCE_TEST_TRUE("true || true");
	SOURCE_TEST_TRUE("true || false");
	SOURCE_TEST_TRUE("false || true");
	SOURCE_TEST_FALSE("false || false");
}
TEST_F(DonutBooleanTest, NotTest)
{
	SOURCE_TEST_FALSE("!true");
	SOURCE_TEST_TRUE("!false");
}


}}


