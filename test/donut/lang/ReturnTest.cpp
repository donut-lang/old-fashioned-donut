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

namespace donut {

TEST(ReturnTest, LiteralTest)
{
	SOURCE_TEST_INT(10, "(func() { if( true ){ return 10; 'xyz' }else{ 'def' }; })();");
}

TEST(ReturnTest, ReturnFunctionTest)
{
	SOURCE_TEST_STR("abc", "(func() { var f=func( x ){ if(x==1){}else{return \"abc\";}; \"def\"  }; f(2); })();");
}

TEST(ReturnTest, ReturnLoopTest)
{
	std::string src(R"delimiter(
			var f=func(acc, loop){
				if(loop <= 0){
					return acc;
				}else{
				};
				return f( acc+loop, loop-1 );
			};
			f(0, 10);
			)delimiter");
	SOURCE_TEST_INT(55, src);
}

}
