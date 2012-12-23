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

#include "DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {

class DonutForTest : public ::testing::Test
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

TEST_F(DonutForTest, CountTest)
{
	SOURCE_TEST_INT(10, "cnt=0;for(i=0;i<10;++i){cnt+=1;};");
}

TEST_F(DonutForTest, ZeroCountTest)
{
	SOURCE_TEST_INT(-12, "cnt=10;for(i=-12;i>0;++i){cnt+=1;};");
}


TEST_F(DonutForTest, WhileTest)
{
	SOURCE_TEST_INT(10, "cnt=0;i=0;while(i<10){++i;cnt+=1;};");
}

TEST_F(DonutForTest, ForWhileTest)
{
	SOURCE_TEST_INT(10, "cnt=0;i=0;for(;i<10;){++i;cnt+=1;};");
}



}}


