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
#include "../Layout.h"

namespace chisa {
namespace tk {
namespace layout {

class MarginLayout : public Layout {
	CHISA_LAYOUT_SUBKLASS(MarginLayout);
	std::weak_ptr<Layout> child_;
	geom::Vector margin_;
	geom::Vector removedSize_;
public:
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
	virtual string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual weak_ptr<Layout> getLayoutByIdImpl(const std::string& id) override;
};

}}}
