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

#pragma once
#include <functional>
#include "Decoder.h"
#include "Encoder.h"
#include "../Exception.h"
#include "../object/Object.h"

namespace chisa {
namespace donut {
namespace native {

template <size_t idx, typename R>
Handler<Object> bindArgumentPure(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<R()> const& funct)
{
	return native::Encoder<R>::exec( heap, funct() );
}

template <size_t idx, typename R, typename U, typename... Args>
Handler<Object> bindArgumentPure(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<R(U val, Args... args)> const& funct)
{
	U const val = native::Decoder<U>::exec( heap, args[idx] );
	std::function<R(Args ... args)> const left ( [funct, val](Args... args) {
		return funct(val, args...);
	});
	return bindArgumentPure<idx + 1, R> (heap, args, left);
}

template <typename R, typename S, typename... Args>
std::function<Handler<Object>(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)> createBindPure(std::function<R(S self, Args... args)> f)
{
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args){
		if (args.size() != sizeof...(Args)) {
			throw DonutException(__FILE__, __LINE__, "this pure native closure needs %d arguments, but %d arguments applied.", sizeof...(Args), args.size());
		}
		std::function<R(Args...)> self_applied ( [f, heap, self ](Args... args_){ return f(native::Decoder<S>::exec( heap, self ), args_...); } );
		return bindArgumentPure<0>(heap, args, self_applied);
	};
}

/**********************************************************************************************************************
 * reactive
 **********************************************************************************************************************/

template <size_t idx, typename R>
std::tuple<Handler<Object>, util::XValue> bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, util::XValue>()> const& funct)
{
	R res;
	util::XValue val;
	std::tie(res, val) = funct();
	return std::tuple<Handler<Object>, util::XValue>(native::Encoder<R>::exec( heap, res ), val);
}

template <size_t idx, typename R, typename U, typename... Args>
std::tuple<Handler<Object>, util::XValue> bindArgumentReactive(Handler<Heap> const& heap, std::vector<Handler<Object> > const& args, std::function<std::tuple<R, util::XValue>(U val, Args... args)> const& funct)
{
	U const val = native::Decoder<U>::exec( heap, args[idx] );
	std::function<std::tuple<R, util::XValue>(Args ... args)> const left ( [funct, val](Args... args) {
		return funct(val, args...);
	});
	return bindArgumentPure<idx + 1, R> (heap, args, left);
}

template <typename R, typename S, typename... Args>
std::function<std::tuple<Handler<Object>, util::XValue>(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args)> createBindReactive(std::function<std::tuple<R, util::XValue>(S self, Args... args)> f)
{
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args){
		if (args.size() != sizeof...(Args)) {
			throw DonutException(__FILE__, __LINE__, "this reactive native closure needs %d arguments, but %d arguments applied.", sizeof...(Args), args.size());
		}
		std::function<std::tuple<R, util::XValue>(Args...)> self_applied ( [f, heap, self ](Args... args_){ return f(native::Decoder<S>::exec( heap, self ), args_...); } );
		return bindArgumentReactive<0>(heap, args, self_applied);
	};
}
}}}
