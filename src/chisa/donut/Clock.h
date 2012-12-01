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
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"

namespace chisa {
namespace donut {

class Donut;

class Clock : public HandlerBody<Clock> {
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	std::vector<unsigned int> clockStack_;
	unsigned int last_;
	unsigned int first_;
	unsigned int now_;
private:
	HandlerW<Donut> donut_;
public:
	Clock( Donut* const donut );
	virtual ~Clock() noexcept = default;
	bool onFree() noexcept { return false; };
public:
	unsigned int now() const noexcept { return this->now_; };
public:
	void tick();
	void discardFuture();
	void discardHistory();
};

}}
