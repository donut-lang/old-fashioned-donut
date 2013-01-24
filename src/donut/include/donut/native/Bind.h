/**
 * Donut
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

#pragma once
#include <functional>
#include <tarte/TypeTrans.h>
#include "../Exception.h"
#include "../object/Object.h"
#include "Decoder.h"
#include "Encoder.h"

namespace donut {
using namespace tarte;

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
-> decltype(createBindPure( ::tarte::makeFunctor(f) ))
{
	return createBindPure( ::tarte::makeFunctor(f) );
}

/**********************************************************************************************************************
 * reactive
 **********************************************************************************************************************/

template <size_t idx, typename __AntiSideEffect, typename R>
std::tuple<Handler<Object>, bool, __AntiSideEffect> bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, bool, __AntiSideEffect>()> const& funct)
{
	R res;
	bool ok;
	__AntiSideEffect val;
	std::tie(res, ok, val) = funct();
	return std::tuple<Handler<Object>, bool, __AntiSideEffect>(native::Encoder<R>::exec( heap, res ), ok, val);
}

template <size_t idx, typename __AntiSideEffect, typename R, typename U, typename... Args>
std::tuple<Handler<Object>, bool, __AntiSideEffect> bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, bool, __AntiSideEffect>(U val, Args... args)> const& funct)
{
	return bindArgumentReactive<idx + 1> (heap, args, std::function<std::tuple<R, bool, __AntiSideEffect>(Args ... args)>( [funct, &heap, &args](Args... largs) {
		return funct(native::Decoder<U>::exec( heap, args[idx] ), largs...);
	}));
}

template <typename __AntiSideEffect,typename R, typename S, typename... Args>
std::function<std::tuple<Handler<Object>, bool, __AntiSideEffect>(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)>
createBindReactive(std::function<std::tuple<R, bool, __AntiSideEffect>(S self, Args... args)> f)
{
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)->std::tuple<Handler<Object>, bool, __AntiSideEffect>{
		if (args.size() != sizeof...(Args)) {
			DONUT_EXCEPTION(Exception, "this reactive native closure needs %d arguments, but %d arguments applied.", sizeof...(Args), args.size());
		}
		return bindArgumentReactive<0>(heap, args, std::function<std::tuple<R, bool, __AntiSideEffect>(Args...)>(
						[f, heap, self ](Args... args_){
							return f(native::Decoder<S>::exec( heap, self ), args_...);
						}
				));
	};
}

template <typename __F>
auto createBindReactive(__F f)
->decltype(createBindReactive(::tarte::makeFunctor(f)))
{
	return createBindReactive(::tarte::makeFunctor(f));
}

}}
