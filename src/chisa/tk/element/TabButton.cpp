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

#include "TabButton.h"
#include "TabCombo.h"

namespace chisa {
namespace tk {


CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TabButton, AbstractToggleButton)
{
	this->margin(geom::Space(0));
	this->padding(geom::Space(0));
}

TabButton::~TabButton() noexcept
{
}

void TabButton::tab(Handler<TabCombo> const& tab)
{
	this->tab_ = tab;
}
void TabButton::element(Handler<Element> const& element)
{
	this->element_ = element;
}

std::string TabButton::toString() const
{
	return ::tarte::format("(TabButton text:\"%s\" %p)", this->text().c_str(), this);
}

void TabButton::checkedImpl(bool const& state)
{
	if(state && this->tab_){
		this->tab_->bringChildToFront(this->element_);
	}
}
bool TabButton::checkedImpl() const noexcept
{
	return this->tab_ && this->tab_->frontChild() == this->element_;
}

}}
