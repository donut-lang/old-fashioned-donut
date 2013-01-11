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

#include "../Angel.h"
#include "../../gl/Color.h"

namespace chisa {
namespace tk {

class HaloServant : public Servant {
private:
	gl::Color color_;
public:
	inline gl::Color const& color() const { return this->color_; };
public:
	HaloServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color);
	virtual ~HaloServant() noexcept = default;
};


}}
