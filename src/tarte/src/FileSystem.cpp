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

#include <tarte/FileSystem.h>
#include <tarte/Exception.h>
#if CHISA_WINDOWS
#include "internal/Win32.h"
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace tarte {
namespace file {
namespace internal {

}

#if CHISA_WINDOWS
static void enumFilesImpl(std::wstring const& dir, std::vector<std::string>& list, bool recursive)
{
	using namespace internal;
	using namespace internal::win32;
	typedef FileConstants<std::wstring> ftype;
	WIN32_FIND_DATAW findFileData;
	HANDLE h = FindFirstFileW(join(dir,L"*.*").c_str(), &findFileData);
	if(h == INVALID_HANDLE_VALUE){
		return;
	}
	do{
		static const std::wstring CurrentDirStr(ftype::CurrentDir);
		static const std::wstring ParentDirStr(ftype::ParentDir);
		std::wstring name(join(dir,findFileData.cFileName));
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if(CurrentDirStr!=findFileData.cFileName && ParentDirStr!=findFileData.cFileName && recursive){
				enumFilesImpl(name, list, recursive);
			}
		}else{
			list.push_back(toUTF8(name));
		}
	}while(FindNextFileW(h, &findFileData) != 0);
	FindClose(h);
}
std::vector<std::string> enumFiles(std::string const& dir, bool recursive)
{
	using namespace internal::win32;
	std::vector<std::string> list;
	enumFilesImpl(toUTF16(dir), list, recursive);
	return list;
}
#else
static void enumFilesImpl(std::string const& dir, std::vector<std::string>& list, bool recursive)
{
	typedef internal::FileConstants<std::string> ftype;
	struct dirent* de;
	DIR* d = opendir(dir.c_str());
	if( !d ){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to open dir: %s", dir.c_str());
	}
	while((de = readdir(d)) != nullptr){
		std::string name = join(dir, de->d_name);
		struct stat64 st;
		if(lstat64(name.c_str(), &st) != 0){
			TARTE_EXCEPTION(Exception, "Failed to lstat64.");
		}
		if(S_IFDIR & st.st_mode){
			static const std::string CurrentDirStr(ftype::CurrentDir);
			static const std::string ParentDirStr(ftype::ParentDir);
			if(CurrentDirStr!=de->d_name && ParentDirStr != de->d_name && recursive){
				enumFilesImpl(name, list, recursive);
			}
		} else {
			list.push_back(name);
		}
	}
	closedir(d);
}

std::vector<std::string> enumFiles(std::string const& dir, bool recursive)
{
	std::vector<std::string> list;
	enumFilesImpl(dir, list, recursive);
	return list;
}
#endif

}}
