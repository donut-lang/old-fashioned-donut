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
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual bool haveOwnImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) const override;
};

class PureNativeClosureObject : public NativeClosureObject {
private:
	std::function<Handler<Object>(Handler<Object> self, Handler<DonutObject> arg)> func_;
public:
	template <typename R, typename... Args>
	PureNativeClosureObject(World* const world, std::string objectProviderName, std::string closureName, std::function<R(Args... args)> func)
	:NativeClosureObject(world, objectProviderName, closureName),func_( native::createBind(func) ){};
	virtual ~PureNativeClosureObject() noexcept {}
public:
	Handler<Object> apply(Handler<Object> self, Handler<DonutObject> arg){ return func_(self,arg); }
};

}}
