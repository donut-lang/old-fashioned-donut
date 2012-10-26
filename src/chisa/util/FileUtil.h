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

template<>
struct FileUtil<std::string>
{
	typedef const char* char_type;
	typedef const std::string string_type;
#if CHISA_WINDOWS
	static constexpr char_type Sep="\\";
#else
	static constexpr char_type Sep = "/";
#endif
	static constexpr char_type CurrentDir = (".");
	static constexpr char_type ParentDir = ("..");
	static string_type CurrentDirStr;
	static string_type ParentDirStr;
};
template<>

struct FileUtil<std::wstring>
{
	typedef const wchar_t* char_type;
	typedef const std::wstring string_type;
#if CHISA_WINDOWS
	static constexpr char_type Sep = L"\\";
#else
	static constexpr char_type Sep = L"/";
#endif
	static constexpr char_type CurrentDir = L".";
	static constexpr char_type ParentDir = L"..";
	static string_type CurrentDirStr;
	static string_type ParentDirStr;
};

}

inline const std::string& join(const std::string& a)
{
	return a;
}
inline const std::wstring& join(const std::wstring& a)
{
	return a;
}

template <typename... Args>
std::string join(const std::string& a, const Args&... left)
{
	using namespace internal;
	return *(a.end()-1) != FileUtil<std::string>::Sep[0] ? a+FileUtil<std::string>::Sep+join(left...) : a+join(left...);
}
template <typename... Args>
std::wstring join(const std::wstring& a, const Args&... left)
{
	using namespace internal;
	return *(a.end()-1) != FileUtil<std::wstring>::Sep[0] ? a+FileUtil<std::wstring>::Sep+join(left...) : a+join(left...);
}

void enumFiles(const std::string& dir, std::set<std::string>& list, bool recursive=false);

}}}
