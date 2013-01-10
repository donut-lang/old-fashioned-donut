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
#include "ProviderDecl.h"

namespace donut {

class PureNativeClosureObject;
class PureNativeClosureProvider final : public HeapProviderBaseT<PureNativeClosureProvider, PureNativeClosureObject> {
public:
	PureNativeClosureProvider( Handler<Heap> const& heap );
	virtual ~PureNativeClosureProvider() noexcept = default;
};

class ReactiveNativeClosureObject;
class ReactiveNativeClosureProvider final : public HeapProviderBaseT<ReactiveNativeClosureProvider, ReactiveNativeClosureObject> {
public:
	ReactiveNativeClosureProvider( Handler<Heap> const& heap );
	virtual ~ReactiveNativeClosureProvider() noexcept = default;
public:
	template <typename __AntisideEffect, typename... Args> inline
	Handler<ReactiveNativeClosureObject> newInstance(Args... args) {
		Handler<ReactiveNativeClosureObject> t ( new ReactiveNativeClosureObject( static_cast<ReactiveNativeClosureProvider*>(this) ) );
		t->bootstrap<__AntisideEffect>(args...);
		return t;
	}
};

}
