/**
 * Tarte
 * Copyright (C) 2013 psi
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

#pragma once
#include <functional>

namespace tarte {

namespace internal {
	template <typename Func>
	struct UnaryFunctionType {
		template <typename R, typename A>
		static auto f(std::function<R(A)>) -> A;
		template <typename R, typename A>
		static auto f(R(*)(A)) -> A;
		template <typename R, typename A>
		static auto g(std::function<R(A)>) -> R;
		template <typename R, typename A>
		static auto g(R(*)(A)) -> R;
		typedef decltype( f( std::declval<Func>()) ) argument_type;
		typedef decltype( g( std::declval<Func>()) ) return_type;
	};
}

template <typename R>
typename internal::UnaryFunctionType<R>::return_type unary_function_return_type(R);

template <typename R>
typename internal::UnaryFunctionType<R>::argument_type unary_function_argument_type(R);

}
