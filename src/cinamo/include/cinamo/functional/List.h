/* coding: utf-8 */
/**
 * CppMonad
 *
 * Copyright 2013, psi
 */

/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, psi
 */
#pragma once
#include <array>
#include <initializer_list>
#include "Base.h"

namespace cinamo {

template <typename A, size_t len>
class List {
private:
	std::array<A, len> spirit;
public:
	template <typename... Args>
	constexpr List(Args... args):spirit( (std::array<A, len>{{ args... }}) ){
	}
private:
	template <typename R, size_t n_, typename F, typename... Args>
	constexpr R call_(typename std::enable_if<(n_ == 0)>::type*,F f, Args... args) const
	{
		return f(args...);
	}
	template <typename R, size_t n_, typename F, typename... Args>
	constexpr R call_(typename std::enable_if<(n_ > 0)>::type*, F f, Args... args) const
	{
		return call_<R,n_-1,F>(nullptr, f, spirit[n_-1], args...);
	}
	template <typename R, typename F, typename... Args>
	constexpr R call_proxy(F f) const
	{
		return call_<R, len>(nullptr, f);
	}
private:
	template <typename R, typename... Args>
	static R resolve(std::function<R(Args...)>);
public:
	template <typename R, typename F>
	constexpr R call_for_struct(F f) const
	{
		return this->call_proxy<R>(f);
	}
	template <typename F>
	constexpr auto call(F f) -> decltype(resolve(makeFunctor(f))) const
	{
		return this->call_proxy<decltype(resolve(makeFunctor(f)))>(f);
	}
private:
	struct _cons_first{
		typedef List<A, len+1> return_type;
		List<A, len> const& self_;
		A const it_;
		constexpr _cons_first(List<A, len> const& self, A it):self_(self), it_(it){}
		template <typename... Args>
		List<A, len+1> operator()(Args... args) const{
			return List<A, len+1>(it_, args...);
		}
	};
public:
	constexpr List<A, len+1> cons(A item) const{
		return call_for_struct<typename _cons_first::return_type>(_cons_first(*this, item));
	}
private:
	template <size_t nlen, size_t idx>
	constexpr List<A, len+nlen> appendI(typename std::enable_if<(idx > 0)>::type*, List<A, len> const& orig, List<A, len+nlen-idx> const& append) const
	{
		return appendI<nlen, idx-1>(nullptr, orig, append.cons(orig[idx-1]));
	}
	template <size_t nlen, size_t idx>
	constexpr List<A, len+nlen> appendI(typename std::enable_if<(idx == 0)>::type*, List<A, len> const& orig, List<A, len+nlen> const& append) const
	{
		return append;
	}
public:
	template <size_t nlen>
	constexpr List<A, len+nlen> append(List<A, nlen> const& append) const{
		return appendI<len, len>(nullptr, *this, append);
	}
private:
	template <size_t start, size_t idx, size_t slen, typename... Args>
	constexpr List<A, slen> subI(typename std::enable_if<(idx <= 0)>::type*, Args... args){
		return List<A, slen>(args...);
	}
	template <size_t start, size_t idx, size_t slen, typename... Args>
	constexpr List<A, slen> subI(typename std::enable_if<(idx > 0)>::type*, Args... args){
		return subI<start, idx-1, slen>(nullptr, spirit[start+idx-1], args...);
	}
public:
	template <size_t start, size_t slen>
	constexpr List<A, slen> sub(){
		return subI<start, slen, slen>(nullptr);
	}
public:
	constexpr A const& operator[](size_t n) const{
		return spirit[n];
	}
};

template <typename A, typename... Left>
constexpr List<A, sizeof...(Left)+1> toList(A const& a, Left... l){
	return List<A, sizeof...(Left)+1>({a, l...});
}


}


