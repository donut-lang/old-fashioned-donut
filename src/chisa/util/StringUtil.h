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
/* FIXME: さきゅばすから取ってきた */

namespace chisa {
namespace util {

std::string format(const std::string& fmt, ...);
std::string formatv(const std::string& fmt, va_list args);
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
std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);

template <typename T>
T parseAsInt(const std::string& str, int radix=0, bool* succeed=nullptr);
template <typename T>
T parseAs(const std::string& str, bool* succeed=nullptr);

std::string encodeBase64( const std::vector<char>& data );
std::string encodeBase64( const std::string& data );
std::string encodeBase64( const char* data, const std::size_t length);
std::vector<char> decodeBase64( const std::string& str );
std::string decodeBase64AsString( const std::string& str );

std::string decodePercent(const std::string& str);
void split(const std::string& str, const std::string& sep, std::vector<std::string>& list);
void split(const std::string& str, const std::string* sep, size_t n, std::vector<std::string>& list);
template <size_t N>
void split(const std::string& str, const std::string (&sep)[N], std::vector<std::string>& list)
{
	split(str, sep, N, list);
}
void splitSpace(const std::string& str, std::vector<std::string>& list);
void splitLine(const std::string& str, std::vector<std::string>& list);
bool startsWith(const std::string& target, const std::string& prefix);
bool endsWith(const std::string& target, const std::string& suffix);

}}
