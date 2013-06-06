/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <functional>
#include <cinamo/TypeTrans.h>
#include <cinamo/functional/Maybe.h>
#include "../Exception.h"
#include "../object/Object.h"
#include "Decoder.h"
#include "Encoder.h"

namespace donut {
using namespace cinamo;

namespace native {

template <size_t idx, typename R>
Handler<Object> bindArgumentPure(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<R()> const& funct)
{
	return native::Encoder<R>::exec( heap, funct() );
}

template <size_t idx, typename R, typename U, typename... Args>
Handler<Object> bindArgumentPure(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<R(U val, Args... args)> const& funct)
{
	return bindArgumentPure<idx + 1, R> (heap, args, std::function<R(Args ... args)>( [funct, &heap, &args](Args... largs) {
		return funct(native::Decoder<U>::exec( heap, args[idx] ), largs...);
	}));
}

template <typename R, typename S, typename... Args>
std::function<Handler<Object>(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)>
createBindPure(std::function<R(S self, Args... args)> f)
{
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)->Handler<Object>{
		if (args.size() != sizeof...(Args)) {
			DONUT_EXCEPTION(Exception, "this pure native closure needs %d arguments, but %d arguments applied.", sizeof...(Args), args.size());
		}
		return bindArgumentPure<0>(heap, args, std::function<R(Args...)>(
				[f, heap, self ](Args... args_){
					return f(native::Decoder<S>::exec( heap, self ), args_...);
				}
			));
	};
}

template <typename __F>
auto createBindPure(__F f)
-> decltype(createBindPure( ::cinamo::makeFunctor(f) ))
{
	return createBindPure( ::cinamo::makeFunctor(f) );
}

/**********************************************************************************************************************
 * reactive
 **********************************************************************************************************************/

template <size_t idx, typename __AntiSideEffect, typename R>
std::tuple<Handler<Object>, Maybe<__AntiSideEffect> >
bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, Maybe<__AntiSideEffect> >()> const& funct)
{
	auto const res = funct();
	return std::tuple<Handler<Object>, Maybe<__AntiSideEffect> >(native::Encoder<R>::exec( heap, std::get<0>(res) ), std::get<1>(res));
}

template <size_t idx, typename __AntiSideEffect, typename R, typename U, typename... Args>
std::tuple<Handler<Object>, Maybe<__AntiSideEffect> > bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, Maybe<__AntiSideEffect> >(U val, Args... args)> const& funct)
{
	return bindArgumentReactive<idx + 1> (heap, args, std::function<std::tuple<R, Maybe<__AntiSideEffect> >(Args ... args)>( [funct, &heap, &args](Args... largs) {
		return funct(native::Decoder<U>::exec( heap, args[idx] ), largs...);
	}));
}

template <typename __AntiSideEffect,typename R, typename S, typename... Args>
std::function<std::tuple<Handler<Object>, Maybe<__AntiSideEffect> >(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)>
createBindReactive(
	std::function<std::tuple<R, Maybe<__AntiSideEffect> >(S self, Args... args)> f
){
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)
			->std::tuple<Handler<Object>, Maybe<__AntiSideEffect> >{
		if (args.size() != sizeof...(Args)) {
			DONUT_EXCEPTION(Exception, "this reactive native closure needs %d arguments, but %d arguments applied.", sizeof...(Args), args.size());
		}
		return bindArgumentReactive<0>(heap, args, std::function<std::tuple<R, Maybe<__AntiSideEffect> >(Args...)>(
						[f, heap, self ](Args... args_){
							return f(native::Decoder<S>::exec( heap, self ), args_...);
						}
				));
	};
}

template <typename __F>
auto createBindReactive(__F f)
->decltype(createBindReactive(::cinamo::makeFunctor(f)))
{
	return createBindReactive(::cinamo::makeFunctor(f));
}

}}
