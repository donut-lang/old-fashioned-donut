/**
 * Tarte
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
#include "Common.h"

#pragma once
/* FIXME: さきゅばすから取ってきた */

namespace tarte {

std::string format(std::string const& fmt, ...);
std::string formatv(std::string const& fmt, va_list args);
#define TOSTR_DEF(type) std::string toString(type const& val, int radix=10);
TOSTR_DEF(int);
TOSTR_DEF(unsigned int);
TOSTR_DEF(long int);
TOSTR_DEF(unsigned long int);
TOSTR_DEF(long long int);
TOSTR_DEF(unsigned long long int);
#undef TOSTR_DEF
std::string toString(const float val);
std::string toString(const double val);
std::string toString(const bool val);
std::string toLower(std::string const& str);
std::string toUpper(std::string const& str);
std::string join(std::vector<std::string> const& lst);
std::string join(std::vector<std::string> const& lst, std::string const& sep);
std::size_t matchString(std::string const& a, std::string const& b);

std::vector<std::string> breakChar(std::string const& str_);

template <typename T>
T parseAs(std::string const& str, int radix, bool* succeed=nullptr);
template <typename T>
T parseAs(std::string const& str, bool* succeed=nullptr);

std::string decodePercent(std::string const& str);
void split(std::string const& str, std::string const& sep, std::vector<std::string>& list);
void split(std::string const& str, const std::string* sep, size_t n, std::vector<std::string>& list);
template <size_t N>
void split(std::string const& str, const std::string (&sep)[N], std::vector<std::string>& list)
{
	split(str, sep, N, list);
}
void splitSpace(std::string const& str, std::vector<std::string>& list);
void splitLine(std::string const& str, std::vector<std::string>& list);
bool startsWith(std::string const& target, std::string const& prefix);
bool endsWith(std::string const& target, std::string const& suffix);

}
