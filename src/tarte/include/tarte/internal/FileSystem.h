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

#include "../Platform.h"
#include <string>
#include <set>

namespace tarte {
namespace file {

namespace internal {

template <class String>
struct FileConstants ;

#if CHISA_WINDOWS
#define CHISA_FILE_SEP "\\"
#define CHISA_FILE_SEPW L"\\"
#else
#define CHISA_FILE_SEP "/"
#define CHISA_FILE_SEPW L"/"
#endif

#define STRING_INNER \
typedef char* char_type;\
typedef const char* const_char_type;\
typedef std::string string_type;\
typedef const std::string const_string_type;\
static constexpr const_char_type Sep = CHISA_FILE_SEP;\
static constexpr const_char_type CurrentDir = ".";\
static constexpr const_char_type ParentDir = "..";

#define WSTRING_INNER \
typedef wchar_t* char_type;\
typedef const wchar_t* const_char_type;\
typedef std::wstring string_type;\
typedef const std::wstring const_string_type;\
static constexpr const_char_type Sep = CHISA_FILE_SEPW;\
static constexpr const_char_type CurrentDir = L".";\
static constexpr const_char_type ParentDir = L"..";

template<> struct FileConstants<std::string>{ STRING_INNER };
template<> struct FileConstants<std::wstring> { WSTRING_INNER };

template<> struct FileConstants<char> { STRING_INNER };
template<> struct FileConstants<wchar_t> { WSTRING_INNER };

template<> struct FileConstants<char*> { STRING_INNER };
template<> struct FileConstants<wchar_t*> { WSTRING_INNER };

template<> struct FileConstants<const char*> { STRING_INNER };
template<> struct FileConstants<const wchar_t*> { WSTRING_INNER };

template<std::size_t N> struct FileConstants<char[N]> { STRING_INNER };
template<std::size_t N> struct FileConstants<wchar_t [N]> { WSTRING_INNER };

#undef STRING_INNER
#undef WSTRING_INNER
#undef CHISA_FILE_SEP
#undef CHISA_FILE_SEPW

}

template <typename T>
constexpr const T& join(const T& a)
{
	return a;
}

}}
