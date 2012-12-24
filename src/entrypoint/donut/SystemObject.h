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
#include <donut/object/Object.h>
#include <donut/object/ReactiveNativeObject.h>

namespace donut_cli {
using namespace tarte;
using namespace donut;

class SystemProvider;
class SystemObject final: public ReactiveNativeObject {
private:
	std::vector<std::string> args_;
public:
	SystemObject(HeapProvider* const provider);
	virtual ~SystemObject() noexcept = default;
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
	virtual XValue onBack(Handler<Heap> const& heap, XValue const& val) override final;
	virtual XValue onForward(Handler<Heap> const& heap, XValue const& val) override final;
	virtual XValue saveImpl( Handler<Heap> const& heap ) override final;
	virtual void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	static Handler<Object> println(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
	static Handler<Object> print(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
	static Handler<Object> args(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
public:
	void bootstrap(Handler<Heap> const& heap);
};

}
