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

class DonutForTest : public ::testing::Test
{
protected:
	Handler<Code> code;
public:
	void SetUp(){
		code = Handler<Code>(new Code());
	}
	void TearDown(){
		code.reset();
	}
};

TEST_F(DonutForTest, CountTest)
{
	unsigned int idx = Parser::fromString("cnt=0;for(i=0;i<10;++i){cnt+=1;};", "<MEM>", 0)->parseProgram(code);
	World world(log_trace, code);
	Machine machine(log_trace, &world);

	Handler<Object> result = machine.start(idx);
	ASSERT_FALSE(result->isNull());
	ASSERT_FALSE(result->isObject());
	ASSERT_TRUE(result->isInt());
	ASSERT_EQ(10, result->toInt(&world));
}

TEST_F(DonutForTest, ZeroCountTest)
{
	unsigned int idx = Parser::fromString("cnt=10;for(i=-12;i>0;++i){cnt+=1;};", "<MEM>", 0)->parseProgram(code);
	World world(log_trace, code);
	Machine machine(log_trace, &world);

	Handler<Object> result = machine.start(idx);
	ASSERT_FALSE(result->isNull());
	ASSERT_FALSE(result->isObject());
	ASSERT_TRUE(result->isInt());
	ASSERT_EQ(-12, result->toInt(&world));
}


}}


