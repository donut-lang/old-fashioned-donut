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

#include "AbstractToggleButton.h"

namespace chisa {
namespace tk {
class TabCombo;

class TabButton: public AbstractToggleButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(TabButton);
private:
	Handler<TabCombo> tab_;
	Handler<Element> element_;
public:
	virtual std::string toString() const override final;
public:
	void tab(Handler<TabCombo> const& tab);
	void element(Handler<Element> const& element);
private:
	virtual void checkedImpl(bool const& state) override final;
	virtual bool checkedImpl() const noexcept  override final;
};

}}
