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
#include "../../util/StringUtil.h"
#include "../Exception.h"
#include "../object/ObjectBase.h"


namespace chisa {
namespace donut {
class World;

namespace native {

template <typename T>
T decode(World* world, Handler<Object> obj);

template <typename T>
Handler<Object> encode(World* world, T obj);

template <size_t idx, typename T>
Object* callWithBind(Object* self, BaseObject* args, std::function<Object*(T)> const& funct)
{
	T s = dynamic_cast<T>(self);
	if(!s){
		throw DonutException(__FILE__, __LINE__, "oops. type mismatched. %s <-> %s", typeid(T).name(), typeid(self).name());
	}
	return funct(s);
}

template <size_t idx, typename T, typename U, typename... Args>
Object* callWithBind(Object* self, BaseObject* args, std::function<Object*(T self, U val, Args... args)> const& funct)
{
	if(!args->have(args->world(), idx)){
		constexpr int _idx = idx+1;
		throw DonutException(__FILE__, __LINE__, "oops. args size mismatched. need more than %d arguments.", _idx);
	}
	U const val = native::decode<U>( args->world(), args->load(args->world(), idx) );;
	std::function<Object*(T self, Args... args)> left = [funct, val](T self, Args... args)->Object*{
		return funct(self, val, args...);
	};
	return callWithBind<idx+1>(self, args, left);
}

template <typename... Args>
std::function<Object*(Object* self, BaseObject* arg)> createBind(std::function<Object*(Args... args)> f)
{
	return [f](Object* self, BaseObject* args)->Object*{
		return callWithBind<0>(self, args, f);
	};
}


}}}
