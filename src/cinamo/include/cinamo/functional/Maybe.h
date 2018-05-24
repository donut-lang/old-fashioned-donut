/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, psi
 */
#pragma once

#include <string>
#include <functional>
#include <cstddef>
#include "../String.h"
#include "../Dynamic.h"

namespace cinamo {

template <typename A>
struct _remove_maybe {
	typedef typename A::value_type value_type;
};

template <typename A> class Maybe;
template <typename A> constexpr Maybe<A> Just(A const& x);
template <typename A> constexpr Maybe<A> Nothing();

template <typename A>
class Maybe {
public:
	bool const isNothing;
	bool const isJust;
	typedef A value_type;
private:
	A val_;
	template <typename A_>
	auto toStringImpl(A_ const& val) -> decltype( cinamo::toString(val) ) const {
		return cinamo::format("<Maybe[%s]: Just %s>", cinamo::demangle<A>().c_str(), cinamo::toString(val).c_str());
	}
	template <typename A_>
	auto toStringImpl(A_ const& val) -> decltype( std::string() ) const {
		return cinamo::format("<Maybe[%s]: Just>", cinamo::demangle<A>().c_str(), cinamo::toString(val).c_str());
	}
public:
	constexpr A value() const{
		return isNothing ? (throw "Cannot get value from Nothing") : val_;
	}
	std::string toString() const{
		return isNothing?
				cinamo::format("<Maybe[%s]: Nothing>", cinamo::demangle<A>().c_str()) :
				toStringImpl<A>(this->val_);
	}
private:
	constexpr Maybe():isNothing(true),isJust(false),val_() {}
	constexpr Maybe(A const& val):isNothing(false),isJust(true),val_(val) {}
	template <typename A_> friend constexpr Maybe<A_> Just(A_ const& x);
	template <typename A_>  friend constexpr Maybe<A_> Nothing();

public:
	~Maybe() = default;
public:
	template <typename F>
	constexpr auto operator>>=(F f) -> Maybe<typename _remove_maybe<decltype(f(val_))>::value_type> {
		return isNothing ? decltype(f(val_))() : f(val_);
	}
	template <typename R>
	constexpr Maybe<R> operator>>(Maybe<R> const& r) {
		return isNothing ? Maybe<R>() : r;
	}
	constexpr Maybe<A> operator ||(Maybe<A> m) {
		return isJust ? *this : m;
	}
	template <typename F>
	constexpr Maybe<A> fmap(F f){
		return isNothing ? *this : Just(f(val_));
	}
	template <typename F>
	constexpr auto ifNothing(F f) -> Maybe<decltype(f())>{
		return isNothing ? Just(f()) : Nothing<decltype(f())>();
	}
	template <typename F>
	constexpr auto ifJust(F f) -> Maybe<decltype(f(val_))>{
		return isJust ? Just(f(val_)) : Nothing<decltype(f(val_))>();
	}
	template <typename F>
	static constexpr std::function<Maybe<A>(A const&)> lift(F f){
		return std::function<Maybe<A>(A const&)>([f](A const& a)->Maybe<A>{
			return Just(f(a));
		});
	}
	template <typename Other>
	constexpr bool operator==(Maybe<Other> const& o){
		return std::is_same<Other, A>::value
				&& (this->isNothing == this->isNothing)
				&& (this->isNothing || this->val_ == o.value());
	}
	template <typename Other>
	constexpr bool operator!=(Maybe<Other> const& o){
		return !this->operator ==(o);
	}
};

template <typename A>
constexpr Maybe<A> Just(A const& x){
	return Maybe<A>(x);
}

template <typename A>
constexpr Maybe<A> Nothing(){
	return Maybe<A>();
}

}
