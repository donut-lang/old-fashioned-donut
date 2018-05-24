/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, psi
 */
#pragma once
#include <string>
#include "../String.h"
#include "../Dynamic.h"
#include "Maybe.h"
#include "Ident.h"

namespace cinamo {

using DefaultBoxFailure = std::string;

template <typename A, typename F> class Box;
class FullT final{};
class EmptyT final{};
class FailureT final{};

template <typename A, typename F = DefaultBoxFailure>
static constexpr Box<A, F> Failure(F const& e) {
	return Box<A, F>(e, static_cast<FailureT*>(nullptr));
}

template <typename A, typename F = DefaultBoxFailure>
static constexpr Box<A, F> Full(A const& a) {
	return Box<A, F>(a, static_cast<FullT*>(nullptr));
}

template <typename A, typename F = DefaultBoxFailure>
static constexpr Box<A, F> Empty() {
	return Box<A, F>(static_cast<EmptyT*>(nullptr));
}

template <typename A, typename F = DefaultBoxFailure>
class Box {
public:
	enum {
		TFull, TEmpty, TFailure
	} const type_;
private:
	A const answer_;
	F const error_;

	constexpr Box(F const& e, FailureT const*const): type_(TFailure), answer_(), error_(e){};
	constexpr Box(A const& a, FullT const*const): type_(TFull), answer_(a), error_(){};
	constexpr Box(A const& a, EmptyT const*const): type_(TFull), answer_(), error_(){};

	template <typename A_, typename F_> friend constexpr Box<A_,F_> Full(A_ const& a);
	template <typename A_, typename F_> friend constexpr Box<A_,F_> Empty();
	template <typename A_, typename F_> friend constexpr Box<A_,F_> Failure(F_ const& e);
public:
	typedef F error_type;
	typedef A answer_type;
public:
	std::string toString() const{
		return
				type_ == TFull ? cinamo::format("<Full[%s][%s]: %s>", cinamo::demangle<F>().c_str(), cinamo::demangle<A>().c_str(), cinamo::toString(answer_).c_str()) :
				type_ == TFailure ? cinamo::format("<Failure[%s][%s]: %s>", cinamo::demangle<F>().c_str(), cinamo::demangle<A>().c_str(), cinamo::toString(error_).c_str()) :
				cinamo::format("<Empty[%s][%s]>", cinamo::demangle<F>().c_str(), cinamo::demangle<A>().c_str(), cinamo::toString(error_).c_str());
	}
public:
	constexpr F error() const{
		return (type_ == TFailure) ? error_ : throw cinamo::format("Cannot get error from %s", toString().c_str());
	}
	constexpr A answer() const{
		return (type_ == TFull) ? answer_ : throw cinamo::format("Cannot get answer from %s", toString().c_str());
	}
public:
	constexpr bool isFull() const { return this->type_ == TFull; }
	constexpr bool isEmpty() const { return this->type_ == TEmpty; }
	constexpr bool isFailure() const { return this->type_ == TFailure; }
public:
	template <typename Fun>
	constexpr auto operator >>=(Fun f)
		-> Box<typename Ident<decltype(f(std::declval<A>()))>::type::answer_type, F> {
		return
			isEmpty() ? Empty<typename Ident<Fun>::type::answer_type, typename Ident<Fun>::type::error_type>() :
			isFailure() ? Failure<typename Ident<Fun>::type::answer_type, F>(this->error_) :
			f(answer_);
	}
	constexpr Box<A, F> operator ||(Box<A,F> const& e) {
		return isFull() ? *this : e;
	}
	template <typename A2>
	constexpr auto operator >>(Box<A2,F> a) -> Box<A2,F> {
		return
			isFull ? a :
			isEmpty ? Empty<A2,F>() :
					Failure<A2,F>(error_);
	}
	template <typename Func>
	constexpr Box<A,F> fmap(Func f){
		return !(isFull()) ? *this : Full(f(answer_));
	}
	template <typename Func>
	constexpr auto ifFull(Func f) -> Maybe<decltype(f(answer_))>{
		return isFull() ? Just(f(answer_)) : Nothing<decltype(f(answer_))>();
	}
	template <typename Func>
	constexpr auto ifFailure(Func f) -> Maybe<decltype(f(error_))>{
		return isFailure() ? Just(f(error_)) : Nothing<decltype(f(error_))>();
	}
	template <typename Func>
	constexpr auto ifEmpty(Func f) -> Maybe<decltype(f())>{
		return isEmpty() ? Just(f()) : Nothing<decltype(f())>();
	}
	~Box() = default;
};

}
