/**
 * Chisa
 * Copyright (C) 2013 psi
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
#include <tuple>
#include <cinamo/Handler.h>
#include <cinamo/functional/Maybe.h>
#include "../object/Object.h"


namespace donut {
using namespace cinamo;

class ReactiveNativeClosure : public HandlerBody<ReactiveNativeClosure>{
protected:
	ReactiveNativeClosure() = default;
public:
	virtual ~ReactiveNativeClosure() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const = 0;
};

class Heap;
template <typename __AntiSideEffect>
class ReactiveNativeClosureBaseT : public ReactiveNativeClosure{
public:
	typedef std::tuple<Handler<Object>, Maybe<__AntiSideEffect> > ResultType;
public:
	typedef std::function<ResultType(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)> Signature;
	typedef ResultType(*Function)(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
private:
	Signature func_;
public:
	ReactiveNativeClosureBaseT(Signature f):func_(f){}
	virtual ~ReactiveNativeClosureBaseT() noexcept {};
public:
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const;
};

}

