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

class DonutObjectTest : public ::testing::Test
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

TEST_F(DonutObjectTest, IntCannotCastToBaseObjectTest)
{
	Handler<Object> obj = world->createInt(10);
	ASSERT_EQ(10, obj->toInt(world));

	Handler<BaseObject> converted;
//	ASSERT_NO_FATAL_FAILURE( obj.tryCast<BaseObject>() );
}

}}


