/**
 * Donut
 * Copyright (C) 2013 psi
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
#include <donut/Patron.h>
#include "SystemProvider.h"

namespace donut_cli {

class SystemPatron final : public donut::Patron {
	int argc_;
	char** argv_;
	Handler<SystemProvider> systemProvider_;
public:
	SystemPatron(int argc, char** argv);
	virtual ~SystemPatron() noexcept = default;
public:
	virtual void onRegisterProvider(Handler<Heap> const& heap) override final;
	virtual void onGlobalObjectInitialized(Handler<Heap> const& heap) override final;
};



}
