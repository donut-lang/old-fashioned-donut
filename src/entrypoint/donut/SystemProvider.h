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
#include <donut/Donut.h>
#include <donut/provider/Provider.h>

namespace donut_cli {
using namespace donut;

class SystemObject;
class SystemProvider final : public ReactiveProviderBaseT<SystemProvider, SystemObject> {
private:
	std::vector<std::string> const args_;
public:
	SystemProvider(Handler<Heap> const& heap, int argc, char** argv);
	virtual ~SystemProvider() noexcept = default;
public:
	inline std::vector<std::string> const& args() const noexcept{ return this->args_; };
};

}
