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
	virtual std::string toStringImpl(Handler<Heap> const& heap) const override final;
	virtual int toIntImpl(Handler<Heap> const& heap) const override final;
	virtual float toFloatImpl(Handler<Heap> const& heap) const override final;
	virtual bool toBoolImpl(Handler<Heap> const& heap) const override final;
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
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
	virtual std::string toStringImpl(Handler<Heap> const& heap) const override final;
	virtual int toIntImpl(Handler<Heap> const& heap) const override final;
	virtual float toFloatImpl(Handler<Heap> const& heap) const override final;
	virtual bool toBoolImpl(Handler<Heap> const& heap) const override final;
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
public:
	void bootstrap( Handler<Heap> const& heap, float const& val );
};

class ReactiveNativeObject : public NativeObject {
private:
	std::vector<std::pair<timestamp_t, util::XValue> > reactions_;
	int index_;
public:
	ReactiveNativeObject( std::string const& providerName );
	virtual ~ReactiveNativeObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap);
	virtual util::XValue save( Handler<Heap> const& heap ) override final;
	virtual void load( Handler<Heap> const& heap, util::XValue const& data ) override final;
private:
	int findIndex( timestamp_t const& t );
protected:
	virtual std::string toStringImpl(Handler<Heap> const& heap) const override final;
	virtual int toIntImpl(Handler<Heap> const& heap) const override final;
	virtual float toFloatImpl(Handler<Heap> const& heap) const override final;
	virtual bool toBoolImpl(Handler<Heap> const& heap) const override final;
public:
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
public:
	void registerReaction( timestamp_t time, util::XValue const& v );
protected:
	virtual std::tuple<bool, util::XValue> onBack(Handler<Heap> const& heap, util::XValue const& val) = 0;
	virtual std::tuple<bool, util::XValue> onForward(Handler<Heap> const& heap, util::XValue const& val) = 0;
	virtual void onFutureDiscarded(Handler<Heap> const& heap) = 0;
	virtual void onHistoryDiscarded(Handler<Heap> const& heap) = 0;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override = 0;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override = 0;
};

class PureNativeClosureObject final : public NativeClosureObject {
public:
	typedef std::function<Handler<Object>(Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& arg)> Signature;
private:
	Signature func_;
public:
	PureNativeClosureObject( std::string const& providerName):NativeClosureObject(providerName){}
	virtual ~PureNativeClosureObject() noexcept {};
	void bootstrap( std::string const& objectProviderName, std::string const& closureName, Signature f );
public:
	Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& arg);
private:
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
};

class ReactiveNativeClosureObject final : public NativeClosureObject {
public:
	typedef std::function<std::tuple<Handler<Object>, bool, util::XValue>(Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& arg)> Signature;
private:
	Signature func_;
public:
	ReactiveNativeClosureObject( std::string const& providerName):NativeClosureObject(providerName){}
	virtual ~ReactiveNativeClosureObject() noexcept {};
	void bootstrap( std::string const& objectProviderName, std::string const& closureName, Signature f );
public:
	Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, Handler<DonutObject> const& arg);
private:
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
};

}}
