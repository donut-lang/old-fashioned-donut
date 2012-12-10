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

template <size_t idx, typename R, typename T>
Handler<Object> callWithBind(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& args, std::function<R(T)> const& funct)
{
	T const s = native::Decoder<T>::exec( heap, self );
	return native::Encoder<R>::exec( heap, funct(s) );
}

template <size_t idx, typename R, typename T, typename U, typename... Args>
Handler<Object> callWithBind(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& args, std::function<R(T self, U val, Args... args)> const& funct)
{
	if( !args->has(heap, idx) ) {
		constexpr int _idx = idx+1;
		throw DonutException(__FILE__, __LINE__, "oops. args size mismatched. need more than %d arguments.", _idx);
	}
	U const val = native::Decoder<U>::exec( heap, args->get(heap, idx) );
	std::function<R(T self, Args... args)> const left = [funct, val](T self, Args... args)->R{
		return funct(self, val, args...);
	};
	return callWithBind<idx+1>(heap, self, args, left);
}

template <typename R, typename... Args>
std::function<Handler<Object>(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& args)> createBind(std::function<R(Args... args)> f)
{
	return [f](const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& args){
		return callWithBind<0>(heap, self, args, f);
	};
}


}}}
