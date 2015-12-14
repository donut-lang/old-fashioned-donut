/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <functional>
#include "ObjectDecl.hpp"
#include "../native/Bind.hpp"

namespace donut {

class PureNativeClosureObject final : public NativeClosureObject {
public:
	typedef std::function<Handler<Object>(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)> Signature;
	typedef Handler<Object>(*Function)(Handler<Heap> const&, Handler<Object> const&, std::vector<Handler<Object> > const&);
private:
	Signature func_;
public:
	PureNativeClosureObject(HeapProvider* const provider):NativeClosureObject(provider){}
	virtual ~PureNativeClosureObject() noexcept {};
	void bootstrap( std::string const& objectProviderName, std::string const& closureName, Signature f );
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override;
private:
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const override final;
};

}
