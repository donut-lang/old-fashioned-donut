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
#include <functional>
#include "../Exception.h"
#include "../code/Closure.h"
#include "../../util/StringUtil.h"
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

template <size_t idx, typename T>
Object* call(Object* self, BaseObject* args, std::function<Object*(T)> const& funct)
{
	T s = dynamic_cast<T>(self);
	if(!s){
		throw DonutException(__FILE__, __LINE__, "oops. type mismatched. %s <-> %s", typeid(T).name(), typeid(self).name());
	}
	return funct(s);
}

template <size_t idx, typename T, typename U, typename... Args>
Object* call(Object* self, BaseObject* args, std::function<Object*(T self, U const& val, const Args&... args)> const& funct)
{
	std::string id(util::toString(idx));
	if(!args->have(args->world(), id)){
		constexpr int _idx = idx+1;
		throw DonutException(__FILE__, __LINE__, "oops. args size mismatched. need more than %d arguments.", _idx);
	}
	const int val = native::decode<U>( args->world(), args->load(args->world(), id) );;
	std::function<Object*(T self, const Args&... args)> left = [funct, val](T self, const Args&... args)->Object*{
		return funct(self, val, args...);
	};
	return call<idx+1>(self, args, left);
}

template <typename T, typename... Args>
std::function<Object*(Object* self, BaseObject* arg)> createBind(std::function<Object*(T self, const Args&... args)> f)
{
	return [f](Object* self, BaseObject* args)->Object*{
		return call<0, T>(self, args, f);
	};
}

class BuiltinNativeClosure : public Object {
private:
	std::function<Object*(Object* self, BaseObject* arg)> func_;
public:
	template <typename... Args>
	BuiltinNativeClosure(World* const world, std::function<Object*(Object* self, const Args&... args)> func):func_( createBind(func) ){};
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
