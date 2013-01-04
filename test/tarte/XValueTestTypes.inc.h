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

{ /* プラス */\
	TYPE const uvar = VALUE;\
	XValue x (uvar);\
	ASSERT_NO_THROW(x.as<TYPE>());\
	ASSERT_ANY_THROW(x.as<NOT_TYPE>());\
	TYPE var;\
	NOT_TYPE nvar;\
	ASSERT_ANY_THROW(x.as(nvar));\
	ASSERT_EQ(VALUE, x.as(var));\
	ASSERT_EQ(VALUE, var);\
}\
{ /* マイナス */\
	TYPE const uvar = -(VALUE);\
	XValue x (uvar);\
	ASSERT_NO_THROW(x.as<TYPE>());\
	ASSERT_ANY_THROW(x.as<NOT_TYPE>());\
	TYPE var;\
	NOT_TYPE nvar;\
	ASSERT_ANY_THROW(x.as(nvar));\
	ASSERT_EQ(static_cast<TYPE>(-(VALUE)), x.as(var));\
	ASSERT_EQ(static_cast<TYPE>(-(VALUE)), var);\
}

