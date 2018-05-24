/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, psi
 */
#pragma once

#include <utility>
#include <type_traits>
#include "Base.h"
#include "Ident.h"
#include "Maybe.h"

namespace cinamo {

namespace either_innenr{
class LeftEnumT;
class RightEnumT;
class ConstexprLeftEnumT;
class ConstexprRightEnumT;

template <typename E, typename U=void>
struct ToString {
	static std::string print(E const& e){
		return "???";
	}
};

template <typename E>
struct ToString<E, decltype(cinamo::toString(std::declval<E>()), void())>
{
	static std::string print(E const& e){
		return cinamo::toString(e);
	}
};

}

#define IS_NONTRIVIAL(E__,A__) typename std::enable_if<!(std::is_trivial<E__>::value && std::is_trivial<A__>::value)>::type
#define IS_TRIVIAL(E__,A__) typename std::enable_if<(std::is_trivial<E__>::value && std::is_trivial<A__>::value)>::type

template <typename E, typename A, typename U=void> class Either;
template <typename E, typename A, IS_NONTRIVIAL(E,A)* = nullptr>
Either<E,A> Left(E const& e) {
	return Either<E,A>(e, static_cast<either_innenr::LeftEnumT*>(nullptr));
}

template <typename E, typename A, IS_NONTRIVIAL(E,A)* = nullptr>
Either<E,A> Right(A const& a){
	return Either<E,A>(a, static_cast<either_innenr::RightEnumT*>(nullptr));
}
template <typename E, typename A, IS_TRIVIAL(E,A)* = nullptr>
constexpr Either<E,A> Left(E const& e) {
	return Either<E,A>(e, static_cast<either_innenr::ConstexprLeftEnumT*>(nullptr));
}

template <typename E, typename A, IS_TRIVIAL(E,A)* = nullptr>
constexpr Either<E,A> Right(A const& a){
	return Either<E,A>(a, static_cast<either_innenr::ConstexprRightEnumT*>(nullptr));
}

template <typename E, typename A, typename U>
class Either {
public:
	bool const isLeft;
	bool const isRight;
private:
	union {
		E const error_;
		A const answer_;
	};
public:
	Either(E const& e, either_innenr::LeftEnumT*)
	:isLeft(true)
	,isRight(false)
	,error_(e)
	{}
	Either(A const& a, either_innenr::RightEnumT*)
	:isLeft(false)
	,isRight(true)
	,answer_(a)
	{}
	Either(Either<E,A> const& e)
	:isLeft(e.isLeft)
	,isRight(e.isRight)
	{
		if( isLeft ) {
			using RE = typename std::remove_cv<E>::type;
			new (static_cast<void*>(const_cast<RE*>(static_cast<const RE*>(&this->error_)))) E( (e.error_) );
		}
		if( isRight ) {
			using RA = typename std::remove_cv<A>::type;
			new (static_cast<void*>(const_cast<RA*>(static_cast<const RA*>(&this->answer_)))) A( (e.answer_) );
		}
	}
public:
	~Either() {
		if(isLeft) {
			this->error_.~E();
		}
		if(isRight) {
			this->answer_.~A();
		}

	}
	template <typename E_, typename A_, typename U_> friend Either<E_,A_> Left(E_ const& x);
	template <typename E_, typename A_, typename U_> friend Either<E_,A_> Right(A_ const& x);
public:
	typedef E error_type;
	typedef A answer_type;
public:
	std::string toString() const{
		using either_innenr::ToString;
		return isRight ?
				cinamo::format("<Right[%s][%s]: %s>", cinamo::demangle<E>().c_str(), cinamo::demangle<A>().c_str(), ToString<A>::print(answer_).c_str()) :
				cinamo::format("<Left[%s][%s]: %s>",  cinamo::demangle<E>().c_str(), cinamo::demangle<A>().c_str(), ToString<E>::print(error_).c_str());
	}
public:
	E const& error() const{
		if(isLeft){
			return error_;
		}
		throw cinamo::format("Cannot get error from %s", toString().c_str());
	}
	A const& answer() const{
		if(isRight){
			return answer_;
		}
		throw cinamo::format("Cannot get answer from %s", toString().c_str());
	}
	E const& left() const {
		return this->error();
	}
	A const& right() const {
		return this->answer();
	}
public:
	template <typename F>
	auto operator >>=(F f)
		-> Either<E, typename Ident<decltype(f(std::declval<A>()))>::type::answer_type> {
		return isLeft ?
			Left<E, typename  Ident<decltype(f(std::declval<A>()))>::type::answer_type>(error_) :
			f(answer_);
	}
	Either<E,A> operator ||(Either<E,A> const& e) {
		return isRight ? *this : e;
	}
	template <typename A2>
	auto operator >>(Either<E,A2> a) -> Either<E,A2> {
		return isRight ? a : Left<E,A2>(error_);
	}
	template <typename F>
	auto fmap(F f) -> Either<E,decltype(f(answer_))>{
		return isLeft ? Left<E,decltype(f(answer_))>(error_) : Right<E,decltype(f(answer_))>(f(answer_));
	}
	template <typename F> Either<E,A>& ifRight(F f) { if(isRight) f(answer_); return *this; }
	template <typename F> Either<E,A>& ifLeft(F f) { if(isLeft) f(error_); return *this; }

	template <typename F> Either<E,A> const& ifRight(F f) const { if(isRight) f(answer_); return *this; }
	template <typename F> Either<E,A> const& ifLeft(F f) const { if(isLeft) f(error_); return *this; }

	template <typename E_,typename A_,typename U_> friend class Either;
	template <typename E_,typename A_>
	constexpr bool operator ==(Either<E_,A_> const& o) const
	{
		return
			this->isRight ? ( std::is_same<A, A_>::value && o.isRight && answer_ == o.answer_ ) :
			this->isLeft ? (std::is_same<E, E_>::value && o.isLeft && error_ == o.error_) : false;
	}
	template <typename E_,typename A_>
	constexpr bool operator !=(Either<E_, A_> const& o) const
	{
		return !(this-> operator ==(o));
	}
	template <typename F>
	Either<E,A> tryOr(F f){
		return isRight ? *this : f(error());
	}
};

template <typename E, typename A>
class Either<E, A, IS_TRIVIAL(E,A) > final {
public:
	bool const isLeft;
	bool const isRight;
private:
	E const error_;
	A const answer_;
public:
	constexpr Either(E const& e, either_innenr::ConstexprLeftEnumT*)
	:isLeft(true)
	,isRight(false)
	,error_(e)
	,answer_()
	{}
	constexpr Either(A const& a, either_innenr::ConstexprRightEnumT*)
	:isLeft(false)
	,isRight(true)
	,error_()
	,answer_(a)
	{}
public:
	template <typename E_, typename A_, typename U_> friend constexpr Either<E_,A_> Left(E_ const& x);
	template <typename E_, typename A_, typename U_> friend constexpr Either<E_,A_> Right(A_ const& x);
public:
	typedef E error_type;
	typedef A answer_type;
public:
	std::string toString() const{
		using either_innenr::ToString;
		return isRight ?
				cinamo::format("<Right[%s][%s]: %s>", cinamo::demangle<E>().c_str(), cinamo::demangle<A>().c_str(), ToString<A>::print(answer_).c_str()) :
				cinamo::format("<Left[%s][%s]: %s>",  cinamo::demangle<E>().c_str(), cinamo::demangle<A>().c_str(), ToString<E>::print(error_).c_str());
	}
public:
	constexpr E const& error() const{
		return isLeft ? error_ : (throw cinamo::format("Cannot get error from %s", toString().c_str()), error_);
	}
	constexpr A const& answer() const{
		return isRight ? answer_ : (throw cinamo::format("Cannot get answer from %s", toString().c_str()),answer_);
	}
	constexpr E const& left() const {
		return this->error();
	}
	constexpr A const& right() const {
		return this->answer();
	}
public:
	template <typename F>
	constexpr auto operator >>=(F f) const
		-> Either<E, typename Ident<decltype(f(std::declval<A>()))>::type::answer_type> {
		return isLeft ?
			Left<E, typename  Ident<decltype(f(std::declval<A>()))>::type::answer_type>(error_) :
			f(answer_);
	}
	constexpr Either<E,A> operator ||(Either<E,A> const& e) const {
		return isRight ? *this : e;
	}
	template <typename A2>
	constexpr auto operator >>(Either<E,A2> a) const -> Either<E,A2> {
		return isRight ? a : Left<E,A2>(error_);
	}
	template <typename F>
	constexpr auto fmap(F f) const -> Either<E,decltype(f(answer_))>{
		return isLeft ? Left<E,decltype(f(answer_))>(error_) : Right<E,decltype(f(answer_))>(f(answer_));
	}
	template <typename F> Either<E,A>& ifRight(F f) { if(isRight) f(answer_); return *this; }
	template <typename F> Either<E,A>& ifLeft(F f) { if(isLeft) f(error_); return *this; }

	template <typename F> Either<E,A> const& ifRight(F f) const { if(isRight) f(answer_); return *this; }
	template <typename F> Either<E,A> const& ifLeft(F f) const { if(isLeft) f(error_); return *this; }

	template <typename E_,typename A_,typename U_> friend class Either;
	template <typename E_,typename A_>
	constexpr bool operator ==(Either<E_,A_> const& o) const
	{
		return
			this->isRight ? ( std::is_same<A, A_>::value && o.isRight && answer_ == o.answer_ ) :
			this->isLeft ? (std::is_same<E, E_>::value && o.isLeft && error_ == o.error_) : false;
	}
	template <typename E_,typename A_>
	constexpr bool operator !=(Either<E_, A_> const& o) const
	{
		return !(this-> operator ==(o));
	}
	template <typename F>
	constexpr Either<E,A> tryOr(F f) const{
		return isRight ? *this : f(error());
	}
};

#undef IS_NONTRIVIAL
#undef IS_TRIVIAL

}
