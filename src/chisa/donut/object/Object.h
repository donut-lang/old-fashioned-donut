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
#include "ObjectBase.h"
#include "../code/Closure.h"
#include "../native/Convert.h"

namespace chisa {
namespace donut {

class StringObject : public BaseObject {
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

class FloatObject : public BaseObject {
private:
	const float val_;
public:
	FloatObject(World* const world, const float val);
	virtual ~FloatObject() noexcept = default;
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
};

class ClosureObject : public BaseObject {
private:
	Handler<Closure> closure_;
public:
	ClosureObject(World* const world, Handler<Closure> clos);
	virtual ~ClosureObject() noexcept = default;
public:
	Handler<Closure> closure() const { return this->closure_; };
	Handler<Object> searchScope(const std::string& str);
};

class BuiltinNativeClosure : public Object {
private:
	std::function<Object*(Object* self, BaseObject* arg)> func_;
public:
	template <typename... Args>
	BuiltinNativeClosure(World* const world, std::function<Object*(Object* self, const Args&... args)> func)
	:func_( native::createBind(func) ){};
	virtual ~BuiltinNativeClosure() noexcept {}
public:
	Object* apply(Object* self, BaseObject* arg){ return func_(self,arg); }
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) override;
};

}}
