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
	StringObject(World* const world, const std::string& str);
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
	FloatObject(World* const world, const float& str);
	virtual ~FloatObject() noexcept = default;
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
};

class NativeClosureObject : public NativeObject {
private:
	std::string const closureName_;
public:
	NativeClosureObject(World* const world, std::string objectProviderName, std::string closureName)
	:NativeObject(world,objectProviderName),closureName_(closureName) {};
	virtual ~NativeClosureObject() noexcept = default;
	std::string closureName() const noexcept { return this->closureName_; };
public:
	virtual std::string toStringImpl() const override;
};

class PureNativeClosureObject : public NativeClosureObject {
private:
	std::function<Handler<Object>(Handler<Object> self, Handler<DonutObject> arg)> func_;
public:
	PureNativeClosureObject(World* const world, std::string objectProviderName, std::string closureName, std::function<Handler<Object>(Handler<Object> self, Handler<DonutObject> arg)> f)
	:NativeClosureObject(world, objectProviderName, closureName), func_(f){}
	virtual ~PureNativeClosureObject() noexcept {}
public:
	Handler<Object> apply(Handler<Object> self, Handler<DonutObject> arg){ return func_(self,arg); }
};

}}
