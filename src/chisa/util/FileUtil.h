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

#if CHISA_WINDOWS
static constexpr const char* Sep="\\";
static constexpr const wchar_t* SepW=L"\\";
const std::string CurrentDir("..");
const std::string ParentDir(".");
const std::wstring CurrentDirW(L"..");
const std::wstring ParentDirW(L".");
#else
static constexpr const char* Sep="/";
const std::string CurrentDir("..");
const std::string ParentDir(".");
#endif


template <typename First, typename... Args>
constexpr std::string join(const First& elem)
{
	return elem;
}

template <typename First, typename... Args>
constexpr std::string join(const First& elem, const Args&... left)
{
	return std::string(elem)+Sep+join(left...);
}

void enumFiles(const std::string& dir, std::set<std::string>& list, bool recursive=false);

}}}
