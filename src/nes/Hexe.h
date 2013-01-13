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
#include "../chisa/Hexe.h"
#include "donut/NesGeistObject.h"

namespace nes {

class Hexe: public chisa::Hexe {
private:
	::tarte::Handler<NesGeistProvider> nesGeistProvider_;
public:
	Hexe(chisa::Logger& log, std::string const& basepath);
	virtual ~Hexe() noexcept;
public:
	::tarte::Handler<NesGeistProvider> nesGeistProvider() const noexcept { return this->nesGeistProvider_; };
public:
	Handler<Hexe> self() { return ::tarte::Handler<Hexe>::__internal__fromRawPointerWithoutCheck(static_cast<Hexe*>(chisa::Hexe::self().get())); };
public:
	virtual void registerGeistProvider( ::tarte::Handler< ::donut::Heap> const& heap ) override;
	virtual void registerWidgets(chisa::tk::WidgetFactory& factory) override;
	virtual void registerElements(chisa::tk::ElementFactory& factory) override;
public:
	virtual chisa::Handler<chisa::WorldGeist> invokeWorldGeist(chisa::HandlerW<chisa::tk::World> world, std::string const& nameOfGeist) override;
};

}
