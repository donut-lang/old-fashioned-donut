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
#include <set>

namespace chisa {
namespace util {
namespace file {

namespace internal {
template <class String>
struct FileUtil ;

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
static constexpr const_char_type Sep=(CHISA_FILE_SEP);\
static constexpr const_char_type CurrentDir = (".");\
static constexpr const_char_type ParentDir = ("..");\
static const string_type CurrentDirStr;\
static const string_type ParentDirStr;

#define WSTRING_INNER \
typedef wchar_t* char_type;\
typedef const wchar_t* const_char_type;\
typedef std::wstring string_type;\
static constexpr const_char_type Sep = CHISA_FILE_SEPW;\
static constexpr const_char_type CurrentDir = L".";\
static constexpr const_char_type ParentDir = L"..";\
static const string_type CurrentDirStr;\
static const string_type ParentDirStr;\

template<> struct FileUtil<std::string>{ STRING_INNER };
template<> struct FileUtil<std::wstring> { WSTRING_INNER };

template<> struct FileUtil<char> { STRING_INNER };
template<> struct FileUtil<wchar_t> { WSTRING_INNER };

template<std::size_t N> struct FileUtil<char[N]> { STRING_INNER };
template<std::size_t N> struct FileUtil<wchar_t [N]> { WSTRING_INNER };

#undef STRING_INNER
#undef WSTRING_INNER
}

template <typename T>
constexpr const T& join(const T& a)
{
	return a;
}

}}}
