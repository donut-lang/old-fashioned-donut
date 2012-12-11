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

#include "LeafElement.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(LeafElement)
{

}

LeafElement::~LeafElement() noexcept
{

}

std::weak_ptr<Element> LeafElement::getChildAt(const std::size_t index) const
{
	return std::weak_ptr<Element>();
}
std::size_t LeafElement::getChildCount() const
{
	return 0;
}
std::weak_ptr<Element> LeafElement::getElementByIdImpl(std::string const& id)
{
	return std::weak_ptr<Element>();
}


}}}
