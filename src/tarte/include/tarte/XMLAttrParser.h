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
#include <string>
#include <tinyxml2.h>

#define DEF_OVERLOD(Klass)\
template <> void parseAttr(std::string const& name, Klass& ptr, Klass const& def, tinyxml2::XMLElement* elm)

namespace tarte {
namespace xml {

template <typename T> void parseAttr(std::string const& name, T& ptr, const T& def, tinyxml2::XMLElement* elm);
DEF_OVERLOD(std::string);
DEF_OVERLOD(float);
DEF_OVERLOD(int);
DEF_OVERLOD(unsigned int);
DEF_OVERLOD(bool);

}}

#undef DEF_OVERLOD
