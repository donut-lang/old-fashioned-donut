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

TEST(DonutFunctionTest, FuncIsReservedWord)
{
	SOURCE_TEST_THROW("func(a,b);");
}

TEST(DonutFunctionTest, LiteralClosureTest)
{
	SOURCE_TEST_INT(3, "func(a,b){a+b}(1,2);");
}

TEST(DonutFunctionTest, ReturnTest)
{
	SOURCE_TEST_INT(3, "var z = func(a,b){a+b;};z(1,2);");
}

TEST(DonutFunctionTest, ReturnWithContTest)
{
	SOURCE_TEST_INT(12, "var z = func(a,b){a+b;};z(1,2);12");
}

TEST(DonutFunctionTest, ArgumentTest)
{
	SOURCE_TEST_INT(3, "var z = func(a,b,c,d){c;};z(1,2,3,4);");
}

TEST(DonutFunctionTest, ScopeTest)
{
	SOURCE_TEST_INT(0, "var a = 1; var f = func(){ a=0; };f(); a;");
}


TEST(DonutFunctionTest, ScopeHideTest)
{
	SOURCE_TEST_INT(1, "var a = 1; var f = func(a){ a=0; };f(1); a;");
}

TEST(DonutFunctionTest, ClosureCountTest)
{
	SOURCE_TEST_INT(2, "var a = 1; var f = func(){ var b = 0; func(){ b++; }; }(); f(); f(); f();");
}

TEST(DonutFunctionTest, RecursiveTest)
{
	SOURCE_TEST_INT(55, "var f = func(a){ if(a>0){ a+f(a-1); }else{ 0; }; };f(10);");
}

}
