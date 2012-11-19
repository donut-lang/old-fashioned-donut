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
	StringObject(Heap* const heap, const std::string& str);
	virtual ~StringObject() noexcept = default;
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
};

class FloatObject: public NativeObject {
private:
	const float value_;
public:
	FloatObject(Heap* const heap, const float& str);
	virtual ~FloatObject() noexcept = default;
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
};


class PureNativeClosureObject : public NativeClosureObject {
private:
	std::function<Handler<Object>(Handler<Object> self, Handler<DonutObject> arg)> func_;
public:
	PureNativeClosureObject(Heap* const heap, std::string objectProviderName, std::string closureName, std::function<Handler<Object>(Handler<Object> self, Handler<DonutObject> arg)> f)
	:NativeClosureObject(heap, objectProviderName, closureName), func_(f){}
	virtual ~PureNativeClosureObject() noexcept {}
public:
	Handler<Object> apply(Handler<Object> self, Handler<DonutObject> arg){ return func_(self,arg); }
};

}}
