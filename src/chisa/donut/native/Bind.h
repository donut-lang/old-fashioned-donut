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
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "../object/Object.h"

namespace chisa {
namespace donut {
namespace native {

template <size_t idx, typename R>
Handler<Object> bindArgument(Handler<Heap> const& heap, Handler<DonutObject> const& args, std::function<R()> const& funct)
{
	return native::Encoder<R>::exec( heap, funct() );
}

template <size_t idx, typename R, typename U, typename... Args>
Handler<Object> bindArgument(Handler<Heap> const& heap, Handler<DonutObject> const& args, std::function<R(U val, Args... args)> const& funct)
{
	if( !args->has(heap, idx) ) {
		constexpr int _idx = idx+1;
		throw DonutException(__FILE__, __LINE__, "oops. args size mismatched. need more than %d arguments.", _idx);
	}
	U const val = native::Decoder<U>::exec( heap, args->get(heap, idx) );
	std::function<R(Args ... args)> const left = [funct, val](Args... args)->R {
		return funct(val, args...);
	};
	return bindArgument<idx + 1, R> (heap, args, left);
}

template <typename R, typename S, typename... Args>
std::function<Handler<Object>(Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& args)> createBind(std::function<R(S self, Args... args)> f)
{
	return [f](Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& args){
		std::function<R(Args...)> self_applied ( [f, heap, self ](Args... args_){ return f(native::Decoder<S>::exec( heap, self ), args_...); } );
		return bindArgument<0>(heap, args, self_applied);
	};
}

}}}
