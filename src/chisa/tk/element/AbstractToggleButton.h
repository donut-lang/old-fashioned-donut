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

#include "AbstractButton.h"

namespace chisa {
namespace tk {

class AbstractToggleButton: public AbstractButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(AbstractToggleButton);
public:
	struct AttrName{
		static std::string const OnForegroundColor;
		static std::string const OnBackgroundColor;
		static std::string const OffForegroundColor;
		static std::string const OffBackgroundColor;
	};
private:
	gl::Color onForegroundColor_;
	gl::Color onBackgroundColor_;
	gl::Color offForegroundColor_;
	gl::Color offBackgroundColor_;
private:
	geom::Box renderOffset_;
public:
	void toggle();
	void checked(bool const& state);
	bool checked() const;
public:
	gl::Color onBackgroundColor() const;
	gl::Color onForegroundColor() const;
	gl::Color offBackgroundColor() const;
	gl::Color offForegroundColor() const;
protected:
	virtual std::string toString() const override;
protected: /* AbstractButtonの実装 */
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual geom::Box measureButtonContent(geom::Box const& constraint) override;
	virtual void layoutButtonContent(geom::Box const& size) override;
	virtual void renderOn(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual void renderOff(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area) override;
	virtual bool isOn() const noexcept override;
	virtual void onClick() override;
protected:
	virtual void checkedImpl(bool const& state) = 0;
	virtual bool checkedImpl() const noexcept = 0;
};

}}
