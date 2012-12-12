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

namespace chisa {
namespace donut {

class ReactiveNativeObject : public NativeObject {
private:
	std::vector<std::pair<timestamp_t, util::XValue> > reactions_;
	int index_; //次に順方向で挿入されるべきインデックスを示す。0なら戻るものがないのを示す。
public:
	ReactiveNativeObject( std::string const& providerName ):NativeObject(providerName), index_(0){}
	virtual ~ReactiveNativeObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap);
	virtual util::XValue save( Handler<Heap> const& heap ) override final;
	virtual void load( Handler<Heap> const& heap, util::XValue const& data ) override final;
private:
	int findUpperIndex( timestamp_t const& t );
	int findLowerIndex( timestamp_t const& t );
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
	virtual util::XValue onBack(Handler<Heap> const& heap, util::XValue const& val) = 0;
	virtual util::XValue onForward(Handler<Heap> const& heap, util::XValue const& val) = 0;
	virtual void onFutureDiscarded(Handler<Heap> const& heap) = 0;
	virtual void onHistoryDiscarded(Handler<Heap> const& heap) = 0;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override = 0;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override = 0;
};

}}
