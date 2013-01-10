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
#include "ObjectDecl.h"
#include "../native/Bind.h"
#include "../native/ReactiveNativeClosureDecl.h"

namespace donut {

// 多態は使えない。implパターンで対応するしかない。
class ReactiveNativeClosureObject final : public NativeClosureObject {
private:
	Handler<ReactiveNativeClosure> spirit_;
public:
	ReactiveNativeClosureObject(HeapProvider* const provider);
	virtual ~ReactiveNativeClosureObject() noexcept = default;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const override final;
public:
	template <typename __AntiSideEffect>
	void bootstrap( std::string const& objectProviderName, std::string const& closureName, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature f );
};

}
