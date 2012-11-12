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
#include "Bind.h"
#include "../object/Object.h"

namespace chisa {
namespace donut {

class PureNativeClosure : public Object {
private:
	World* world_;
	std::function<Handler<Object>(Handler<Object> self, Handler<BaseObject> arg)> func_;
public:
	template <typename R, typename... Args>
	PureNativeClosure(World* const world, std::function<R(Args... args)> func)
	:world_(world), func_( native::createBind(func) ){};
	virtual ~PureNativeClosure() noexcept {}
public:
	Handler<Object> apply(Handler<Object> self, Handler<BaseObject> arg){ return func_(self,arg); }
	virtual std::string toStringImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual bool haveOwnImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) const override;
};

}}
