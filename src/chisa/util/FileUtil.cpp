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

#include "FileUtil.h"
#include "../logging/Exception.h"
#if CHISA_WINDOWS
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace chisa {
namespace util {
namespace file {
namespace internal {

}

#if CHISA_WINDOWS
static std::wstring toUTF16(const std::string& str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);
	wchar_t* buf = new wchar_t[size+1];
	if(size != MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1)){
		throw chisa::logging::Exception(__FILE__, __LINE__, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::wstring ret(buf);
	delete [] buf;
	return ret;
}
static std::string toUTF8(const std::wstring& str)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0, nullptr, nullptr);
	char* buf = new char[size+1];
	if(size != WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1, nullptr, nullptr)){
		throw chisa::logging::Exception(__FILE__, __LINE__, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::string ret(buf);
	delete [] buf;
	return ret;
}
static void enumFiles(const std::wstring& dir, std::set<std::string>& list, bool recursive)
{
	using namespace internal;
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
				enumFiles(name, list, recursive);
			}
		}else{
			list.insert(toUTF8(name));
		}
	}while(FindNextFileW(h, &findFileData) != 0);
	FindClose(h);
}
void enumFiles(const std::string& dir, std::set<std::string>& list, bool recursive)
{
	return enumFiles(toUTF16(dir), list, recursive);
}
#else
void enumFiles(const std::string& dir, std::set<std::string>& list, bool recursive)
{
	using namespace internal;
	typedef FileConstants<std::string> ftype;
	struct dirent* de;
	DIR* d = opendir(dir.c_str());
	while((de = readdir(d)) != nullptr){
		std::string name = join(dir,ftype::Sep,de->d_name);
		struct stat64 st;
		if(lstat64(name.c_str(), &st) != 0){
			throw logging::Exception(__FILE__, __LINE__, "Failed to lstat64.");
		}
		if(S_IFDIR & st.st_mode){
			static const std::string CurrentDirStr(ftype::CurrentDir);
			static const std::string ParentDirStr(ftype::ParentDir);
			if(CurrentDirStr!=de->d_name && ParentDirStr != de->d_name && recursive){
				enumFiles(name, list, recursive);
			}
		}else{
			list.insert(name);
		}
	}
	closedir(d);
}
#endif

}}}
