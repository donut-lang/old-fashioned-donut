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
#include "Object.h"
#include <functional>
#include "../native/Bind.h"

namespace chisa {
namespace donut {

class StringObject final: public NativeObject {
private:
	const std::string str_;
public:
	StringObject(std::string const& providerName):NativeObject(providerName){};
	virtual ~StringObject() noexcept = default;
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override final;
	virtual int toIntImpl(const Handler<Heap>& heap) const override final;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override final;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override final;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override final;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override final;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
public:
	void bootstrap( Handler<Heap> const& heap, std::string const& val );
};

class FloatObject final: public NativeObject {
private:
	const float value_;
public:
	FloatObject(std::string const& providerName):NativeObject(providerName),value_(NAN){};
	virtual ~FloatObject() noexcept = default;
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override final;
	virtual int toIntImpl(const Handler<Heap>& heap) const override final;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override final;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override final;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override final;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override final;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
public:
	void bootstrap( Handler<Heap> const& heap, float const& val );
};


class PureNativeClosureObject final : public NativeClosureObject {
public:
	typedef std::function<Handler<Object>(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg)> Signature;
private:
	Signature func_;
public:
	PureNativeClosureObject( std::string const& providerName):NativeClosureObject(providerName){}
	virtual ~PureNativeClosureObject() noexcept {}
	void bootstrap( std::string const& objectProviderName, std::string const& closureName, Signature f );
public:
	Handler<Object> apply(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg){
		return func_(heap, self,arg);
	}
private:
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
};

}}
