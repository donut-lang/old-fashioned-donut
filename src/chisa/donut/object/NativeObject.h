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

class StringObject: public NativeObject {
private:
	const std::string str_;
public:
	StringObject(const Handler<Heap>& heap, const std::string& str);
	virtual ~StringObject() noexcept = default;
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override;
	virtual int toIntImpl(const Handler<Heap>& heap) const override;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override;
};

class FloatObject: public NativeObject {
private:
	const float value_;
public:
	FloatObject(const Handler<Heap>& heap, const float& str);
	virtual ~FloatObject() noexcept = default;
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override;
	virtual int toIntImpl(const Handler<Heap>& heap) const override;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override;
};


class PureNativeClosureObject : public NativeClosureObject {
private:
	std::function<Handler<Object>(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg)> func_;
public:
	PureNativeClosureObject(const Handler<Heap>& heap, std::string objectProviderName, std::string closureName, std::function<Handler<Object>(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg)> f)
	:NativeClosureObject(heap, objectProviderName, closureName), func_(f){}
	virtual ~PureNativeClosureObject() noexcept {}
public:
	Handler<Object> apply(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg){
		return func_(heap, self,arg);
	}
};

}}
