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

namespace chisa {
namespace tk {

class AngelWidgetTarget : public AngelTarget {
public:
	AngelWidgetTarget(Handler<Angel> const& angel, std::string const& id, std::string const& guide);
	virtual ~AngelWidgetTarget() noexcept = default;
private:
	std::string const id_;
	HandlerW<Widget> widget_;
	std::string const guide_;
public:
	Handler<AngelWidgetTarget> self() noexcept;
private:
	virtual geom::Area findScreenAreaImpl();
	virtual Handler<AngelWidgetTarget> matchToWidgetTarget(std::string const& widgetId, std::string const& widgetGuide) noexcept override final;
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
private:
	virtual void onAttatchedImpl() override final;
	virtual void renderImpl(gl::Canvas& canvas) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual geom::Box reshapeImpl(geom::Area const& area) override final;
};

}}
