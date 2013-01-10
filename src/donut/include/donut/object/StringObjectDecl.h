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
#include "ObjectDecl.h"

namespace donut {

class StringObject final: public NativeObject {
private:
	const std::string str_;
public:
	StringObject(HeapProvider* const provider);
	virtual ~StringObject() noexcept = default;
public:
	inline std::string const& value() const noexcept { return this->str_; };
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
	virtual std::string printImpl(Handler<Heap> const& heap) const override final;
private:
	virtual Handler<const StringObject> toStringObjectImpl() const override final;
	virtual void onBackNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onForwardNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardHistoryNotifyImpl(Handler<Heap> const& heap) override final;
	virtual void onDiscardFutureNotifyImpl(Handler<Heap> const& heap) override final;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	void bootstrap( Handler<Heap> const& heap, std::string const& val );
};

}
