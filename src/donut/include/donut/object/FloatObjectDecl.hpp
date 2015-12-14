/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ObjectDecl.hpp"

namespace donut {

class FloatObject final: public NativeObject {
private:
	const float value_;
public:
	FloatObject(HeapProvider* const provider);
	virtual ~FloatObject() noexcept = default;
public:
	inline float const& value() const noexcept { return this->value_; };
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
	virtual std::string printImpl(Handler<Heap> const& heap) const override final;
private:
	virtual Handler<const FloatObject> toFloatObjectImpl() const override final;
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	void bootstrap( Handler<Heap> const& heap, float const& val );
};

}
