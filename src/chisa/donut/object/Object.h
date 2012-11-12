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

namespace chisa {
namespace donut {

class StringObject : public BaseObject {
private:
	const std::string str_;
public:
	StringObject(World* const world);
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
	FloatObject(World* const world);
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
	ClosureObject(World* const world, Handler<Closure> clos, Handler<Object> scope=Handler<Object>());
	virtual ~ClosureObject() noexcept = default;
public:
	Handler<Closure> closure() const { return this->closure_; };
};

}}
