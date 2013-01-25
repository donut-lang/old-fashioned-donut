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

class ContentUnderlineServant : public Servant {
private:
	gl::Color color_;
	std::string docId_;
public:
	inline gl::Color const& color() const { return this->color_; };
public:
	ContentUnderlineServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color, std::string const& docId);
	virtual ~ContentUnderlineServant() noexcept = default;
public:
	Handler<ContentUnderlineServant> self() noexcept;
private:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
};

}}