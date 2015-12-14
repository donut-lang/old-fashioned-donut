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

#include "../../TestCommon.hpp"
#include "../../chisa/tk/Task.hpp"
#include <tinyxml2.h>
#include <typeinfo>

namespace chisa {
namespace tk {

TEST(TaskHandlerTest, RegisterTest)
{
	TaskHandler handler(log_trace);
	int x = 0;
	ASSERT_EQ( 0, x );
	std::function<bool()> l([&x]()->bool{
		x+=1;
		return false;
	});
	ASSERT_NO_THROW(handler.send(l));
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_EQ( 1, x );

	//falseを返すときはもう削除される
	ASSERT_NO_THROW(handler.run(10));
	ASSERT_EQ( 1, x );
}

TEST(TaskHandlerTest, RegisterRecycledTest)
{
	TaskHandler handler(log_trace);
	int x = 0;
	ASSERT_EQ( 0, x );
	std::function<bool()> l([&x]()->bool{
		x+=1;
		return true;
	});
	ASSERT_NO_THROW(handler.send(l));
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_EQ( 1, x );

	ASSERT_NO_THROW(handler.run(10));
	ASSERT_EQ( 2, x ); //trueを返すと何度でも呼ばれる
}

TEST(TaskHandlerTest, RegisterVoidLambdaTest)
{
	TaskHandler handler(log_trace);
	int x = 0;
	ASSERT_EQ( 0, x );
	std::function<void()> l([&x]()->void{
		x+=1;
	});
	ASSERT_NO_THROW(handler.send(l));
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_EQ( 1, x );

	ASSERT_NO_THROW(handler.run(10));
	ASSERT_EQ( 1, x ); //voidを返すのはfalseと同じ
}

TEST(TaskHandlerTest, RegisterRawLambdaTest)
{
	TaskHandler handler(log_trace);
	int x = 0;
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.send([&x]()->bool{
		x+=1;
		return false;
	}));
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_EQ( 1, x );

	ASSERT_NO_THROW(handler.run(10));
	ASSERT_EQ( 1, x ); //voidを返すのはfalseと同じ
}

TEST(TaskHandlerTest, RegisterRawLambdaVoidTest)
{
	TaskHandler handler(log_trace);
	int x = 0;
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.send([&x]()->void{
		x+=1;
	}));
	ASSERT_EQ( 0, x );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_EQ( 1, x );

	ASSERT_NO_THROW(handler.run(10));
	ASSERT_EQ( 1, x ); //voidを返すのはfalseと同じ
}

TEST(TaskHandlerTest, RegisterFullLambda)
{
	TaskHandler handler(log_trace);
	float f = 0;
	ASSERT_FLOAT_EQ( 0, f );
	ASSERT_NO_THROW(handler.send([&f](float n)->bool{
		f+=n;
		return true;
	}));
	ASSERT_FLOAT_EQ( 0, f );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_FLOAT_EQ( 10, f );

	ASSERT_NO_THROW(handler.run(10.2));
	ASSERT_FLOAT_EQ( 20.2, f );
}
TEST(TaskHandlerTest, RegisterFullFalseLambda)
{
	TaskHandler handler(log_trace);
	float f = 0;
	ASSERT_FLOAT_EQ( 0, f );
	ASSERT_NO_THROW(handler.send([&f](float n)->bool{
		f+=n;
		return false;
	}));
	ASSERT_FLOAT_EQ( 0, f );
	ASSERT_NO_THROW(handler.run(10));

	ASSERT_FLOAT_EQ( 10, f );

	ASSERT_NO_THROW(handler.run(10.2));
	ASSERT_FLOAT_EQ( 10, f );
}

}}
