/**
 * Saccubus
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

#include <cstdarg>
#include <string>
#include <vector>

#pragma once

namespace tarte {
namespace base64 {

std::string encode( std::vector<char> const& data );
std::string encode( std::string const& data );
std::string encode( const char* data, const std::size_t length);
std::vector<char> decode( std::string const& str );
std::string decodeAsString( std::string const& str );

}}
