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
#include "ReactiveNativeClosureObjectDecl.h"
#include "../native/ReactiveNativeClosure.h"

namespace donut {

template <typename __AntiSideEffect>
void ReactiveNativeClosureObject::bootstrap( std::string const& objectProviderName, std::string const& closureName, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature f )
{
	this->NativeClosureObject::bootstrap(objectProviderName, closureName);
	this->spirit_ = Handler<ReactiveNativeClosure>(new ReactiveNativeClosureBaseT<__AntiSideEffect>(f));
}

}
