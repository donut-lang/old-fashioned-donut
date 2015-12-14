/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "ObjectDecl.hpp"

namespace donut {
using namespace cinamo;

class DonutClosureObject final : public DonutObject {
private:
	Handler<Source> const src_;
	unsigned int const index_;
	Handler<Closure> const asm_;
public:
	DonutClosureObject(HeapProvider* const provider);
	virtual ~DonutClosureObject() noexcept = default;
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
public:
	inline Handler<Source> const& source() const { return this->src_; };
	inline Handler<Closure> const& closureCode() const { return this->asm_; };
public:
	virtual Handler<DonutClosureObject> tryCastToDonutClosureObjectImpl() override final;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Source> const& src, unsigned int const& closureIndex, Handler<Object> const& scope);
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

}
