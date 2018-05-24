/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

namespace cinamo {

struct Unit final{
	operator void(){
		return void();
	}
	constexpr bool operator==(Unit const&) { return true; }
	constexpr bool operator!=(Unit const&) { return false; }
};

}

#include <functional>
/* Make functor */
namespace cinamo {

namespace internal {

template <typename __Lambda, typename Overload = void>
struct __LambdaToFunctor;

template <typename __Lambda>
struct __LambdaToFunctor<__Lambda, typename std::enable_if<std::is_class<__Lambda>::value>::type> {
private:
	template <typename R, typename S, typename... Args>
	static auto f( R(S::*_g_)(Args...) ) -> std::function<R(Args...)>;
	template <typename R, typename S, typename... Args>
	static auto f( R(S::*_g_)(Args...) const ) -> std::function<R(Args...)>;
public:
	typedef decltype(f(&__Lambda::operator())) Functor;
};

}

template <typename R, typename... Args>
auto makeFunctor( std::function<R(Args...)> const& f ) -> std::function<R(Args...)> const& {
	return f;
}

template <typename __F>
auto makeFunctor( __F f, typename std::enable_if<std::is_class<__F>::value>::type* = nullptr )
-> typename internal::__LambdaToFunctor<__F>::Functor{
	return typename internal::__LambdaToFunctor<__F>::Functor(f);
}

template <typename R, typename S, typename... Args>
std::function<R(S*, Args...)> makeFunctor(R(S::*f)(Args...))
{
	return std::function<R(S*, Args...)>(f);
}

template <typename R, typename S, typename... Args>
std::function<R(S const*, Args...)> makeFunctor(R(S::*f)(Args...) const)
{
	return std::function<R(S const*, Args...)>(f);
}

template <typename R, typename... Args>
std::function<R(Args...)> makeFunctor(R(*f)(Args...))
{
	return std::function<R(Args...)>(f);
}



}
