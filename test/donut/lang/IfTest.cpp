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

TEST(DonutIfTest, TrueTest)
{
	SOURCE_TEST_INT(1, "if(1==1){1;}else{2;};");
}

TEST(DonutIfTest, FalseTest)
{
	SOURCE_TEST_INT(2, "if(1!=1){1;}else{2;};");
}

TEST(DonutIfTest, ElifTest)
{
	SOURCE_TEST_INT(3, "var z=3;if(z==1){1;}else if(z==2){2;}else if(z==3){3;}else{4;};");
}


}
