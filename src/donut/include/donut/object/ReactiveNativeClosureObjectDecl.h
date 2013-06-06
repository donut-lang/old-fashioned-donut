/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
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
