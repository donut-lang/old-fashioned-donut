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

#ifndef _CXX_CHISA_UTIL_FILEUTIL_H_
#define _CXX_CHISA_UTIL_FILEUTIL_H_

namespace chisa {
namespace util {

class FileUtil {
public:
#if defined(__WIN32__) || defined(__WIN64__)
	static constexpr const char* Sep="\\";
#else
	static constexpr const char* Sep="/";
#endif
public:
	FileUtil();
	virtual ~FileUtil();
};

}}
#endif /* INCLUDE_GUARD */
