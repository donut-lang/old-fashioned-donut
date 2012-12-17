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

namespace chisa {
namespace donut {
class DonutClosureObject final : public DonutObject {
private:
	Handler<Source> const src_;
	unsigned int const index_;
	Handler<Closure> const asm_;
public:
	DonutClosureObject(std::string const& providerName);
	virtual ~DonutClosureObject() noexcept = default;
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
public:
	inline Handler<Source> const& source() const { return this->src_; };
	inline Handler<Closure> const& closureCode() const { return this->asm_; };
public:
	virtual Handler<DonutClosureObject> tryCastToDonutClosureObjectImpl() override final;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Source> const& src, unsigned int const& closureIndex, Handler<Object> const& scope);
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override final;
};

}}
