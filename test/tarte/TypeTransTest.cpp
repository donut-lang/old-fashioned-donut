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
#include "../TestCommon.h"
#include <tarte/TypeTrans.h>

namespace tarte {

TEST(TypeTransTest, Lambda2Functor){
	auto z = [](){};
	static_assert(
			std::is_same<
				decltype(makeFunctor( z )),
				std::function<void()>
				>::value, "cannot deduce std::function");

	auto d = [](int x){ return x*x; };
	static_assert(
			std::is_same<
				decltype(makeFunctor( d )),
				std::function<int(int)>
				>::value, "cannot deduce std::function");

	std::function<int(int)> f = makeFunctor( [](int x){ return x*x; } );
	ASSERT_EQ( 4, f(2) );
}

TEST(TypeTransTest, Functor2Functor){
	std::function<bool(bool)> _not ( [](bool a){ return !a; } );
	static_assert(
			std::is_same<
				decltype(makeFunctor( _not )),
				std::function<bool(bool)> const&
				>::value, "cannot deduce std::function");

	ASSERT_FALSE( _not(true) );
	ASSERT_TRUE( makeFunctor(_not)(false) );
}

TEST(TypeTransTest, Object2Functor){
	class Test {
	public:
		bool operator()(int a, int b, int c) const{
			return a==b && b == c && c == a;
		}
	};
	static_assert(
			std::is_same<
				decltype(makeFunctor(Test())),
				std::function<bool(int,int,int)> >::value, "deduce failed.");
	auto f = makeFunctor(Test());
	ASSERT_FALSE( f(1,2,3) );
	ASSERT_TRUE( f(1,1,1) );
}

TEST(TypeTransTest, Method2Functor){
	class Test {
	public:
		bool call(float x, float y){
			return x*y > 0;
		}
		bool constCall(float x, float y) const{
			return x*y > 0;
		}
	};
	static_assert(
			std::is_same<
				decltype(makeFunctor(&Test::call)),
				std::function<bool(Test*, float, float)> >::value, "deduce failed.");
	static_assert(
			std::is_same<
				decltype(makeFunctor(&Test::constCall)),
				std::function<bool(Test const*, float, float)> >::value, "deduce failed.");
}

namespace {
int func( int a, int b ){
	return a*b;
}

TEST(TypeTransTest, Function2Functor){
	static_assert(
			std::is_same<
				decltype(makeFunctor(func)),
				std::function<int(int, int)> >::value, "deduce failed.");

	auto f = makeFunctor(func);
	ASSERT_EQ(12, f(3,4));
}
}

}
