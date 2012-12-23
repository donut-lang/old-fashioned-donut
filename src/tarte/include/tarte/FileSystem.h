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

#include "Platform.h"
#include <string>
#include <vector>
#include "internal/FileSystem.h"

namespace tarte {
namespace file {

template <typename T, typename... Args>
constexpr typename internal::FileConstants<T>::string_type join(const T& a, Args const&... left)
{
	return typename internal::FileConstants<T>::string_type(a)+internal::FileConstants<T>::Sep+join(left...);
}

std::vector<std::string> enumFiles(std::string const& dir, bool recursive=false);

}}
