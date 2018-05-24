/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "Common.h"
#include "functional/Base.h"
#include <functional>
#include <utility>

namespace cinamo {

template <int N, typename R_, typename... Args>
struct GetArgsOf{
	typedef typename GetArgsOf<N-1, Args...>::type type;
};

template <typename R, typename... Args>
struct GetArgsOf<0, R, Args...>{
	typedef R type;
};

template <typename Fun>
struct FuncTypeOf {
	template <typename R, typename... Args>
	static auto resultType(std::function<R (Args ...) > const& f) -> R;
	template <int N, typename R, typename... Args>
	static auto argType(std::function<R (Args ...) > const& f) -> typename GetArgsOf<N, Args...>::type;

	template <typename R, typename... Args>
	static auto argTypeToTuple(std::function<R (Args ...) > const& f) -> typename std::tuple<Args...>;

	typedef decltype( resultType( makeFunctor(std::declval<Fun>())) ) Result;
	typedef decltype( argTypeToTuple( makeFunctor(std::declval<Fun>())) ) ArgTuple;
	template <int N>
	struct Arg{
		typedef decltype( argType<N>( makeFunctor(std::declval<Fun>())) ) Type;
	};
};

}
