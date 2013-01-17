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

#include "../Element.h"
#include "Button.h"

namespace chisa {
namespace tk {

class ClockButton: public ClickButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(ClockButton);
private:
	struct AttrName{
		const static std::string Operation;
	};
public:
	enum ClockOperation{
		Back,
		Forward
	};
private:
	enum ClockOperation operation_;
public:
	virtual std::string toString() const override;
private:
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual void onClick() override;
};

}}
