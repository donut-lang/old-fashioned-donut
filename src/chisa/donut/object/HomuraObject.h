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
#include "../../util/VectorMap.h"

namespace chisa {
namespace donut {

class HomuraObject final: public NativeObject {
private:
	util::VectorMap<std::string, Object*> slot_;
public:
	HomuraObject(std::string const& providerName);
	virtual ~HomuraObject() noexcept = default;
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
protected:	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
public:
	virtual bool hasImpl(Handler<Heap> const& heap, std::string const& name) const override final;
	virtual bool hasOwnImpl(Handler<Heap> const& heap, std::string const& name) const override final;
	virtual Handler<Object> setImpl(Handler<Heap> const& heap, std::string const& name, Handler<Object> const& obj) override final;
	virtual Handler<Object> getImpl(Handler<Heap> const& heap, std::string const& name) const override final;
	virtual void markImpl(Handler<Heap> const& heap, int color) override final;
public:
	void bootstrap( Handler<Heap> const& heap );
};

}}
