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
#include "../Handler.h"
#include <vector>

namespace chisa {
namespace donut {

class Clock : public HandlerBody<Clock> {
	std::vector<unsigned int> clockStack_;
	unsigned int now_;
public:
	Clock();
	virtual ~Clock() noexcept = default;
	bool onFree() noexcept { return false; };
public:
	unsigned int now() const noexcept { return this->now_; };
};

}}