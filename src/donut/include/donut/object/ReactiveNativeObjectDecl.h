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
#include <tarte/XArchiver.h>
#include "ObjectDecl.h"

namespace donut {

class ReactiveNativeObject : public NativeObject {
protected:
	ReactiveNativeObject(HeapProvider* const provider):NativeObject(provider){}
	virtual ~ReactiveNativeObject() noexcept = default;
protected:
	std::string reprImpl(Handler<Heap> const& heap) const override;
};

template <typename __Derived, typename __Provider, typename AntiSideEffect>
class ReactiveNativeObjectBaseT;

template <typename __AntiSideEffect=XValue>
class ReactiveNativeObjectT : public ReactiveNativeObject {
protected:
	typedef __AntiSideEffect AntiSideEffect;
	typedef ReactiveNativeObjectT<__AntiSideEffect> Super;
	typedef std::tuple<bool, __AntiSideEffect> ResultType;
private:
	std::vector<std::pair<timestamp_t, __AntiSideEffect> > reactions_;
	int index_; //次に順方向で挿入されるべきインデックスを示す。0なら戻るものがないのを示す。
private:
	template <typename __Derived, typename __Provider, typename _AntiSideEffect> friend class ReactiveNativeObjectBaseT;
	ReactiveNativeObjectT(HeapProvider* const provider);
	virtual ~ReactiveNativeObjectT() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap);
	virtual XValue save( Handler<Heap> const& heap ) override final;
	virtual void load( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	int inline findUpperIndex( timestamp_t const& t );
	int findLowerIndex( timestamp_t const& t );
private:
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
public:
	void registerReaction( timestamp_t time, AntiSideEffect const& v );
protected:
	virtual void onFutureDiscarded(Handler<Heap> const& heap) {};
	virtual void onHistoryDiscarded(Handler<Heap> const& heap) {};
protected:
	virtual ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val) = 0;
	virtual ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val) = 0;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override = 0;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override = 0;
};


template <typename __Derived, typename __Provider, typename __AntiSideEffect=XValue>
class ReactiveNativeObjectBaseT : public ReactiveNativeObjectT<__AntiSideEffect> {
protected:
	typedef __AntiSideEffect AntiSideEffect;
	typedef ReactiveNativeObjectBaseT<__Derived, __Provider, __AntiSideEffect> Super;
	typedef std::tuple<bool, __AntiSideEffect> ResultType;
	ReactiveNativeObjectBaseT(HeapProvider* const provider)
	:ReactiveNativeObjectT<AntiSideEffect>(provider){}
	virtual ~ReactiveNativeObjectBaseT() noexcept = default;
protected:
	virtual ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val) override = 0;
	virtual ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val) override = 0;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override = 0;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override = 0;
};
}
