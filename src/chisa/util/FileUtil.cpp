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
const FileUtil<std::string>::string_type FileUtil<std::string>::CurrentDirStr(FileUtil<std::string>::CurrentDir);
const FileUtil<std::string>::string_type FileUtil<std::string>::ParentDirStr(FileUtil<std::string>::ParentDir);
const FileUtil<char>::string_type FileUtil<char>::CurrentDirStr(FileUtil<char>::CurrentDir);
const FileUtil<char>::string_type FileUtil<char>::ParentDirStr(FileUtil<char>::ParentDir);
const FileUtil<std::wstring>::string_type FileUtil<std::wstring>::CurrentDirStr(FileUtil<std::wstring>::CurrentDir);
const FileUtil<std::wstring>::string_type FileUtil<std::wstring>::ParentDirStr(FileUtil<std::wstring>::ParentDir);
const FileUtil<wchar_t>::string_type FileUtil<wchar_t>::CurrentDirStr(FileUtil<wchar_t>::CurrentDir);
const FileUtil<wchar_t>::string_type FileUtil<wchar_t>::ParentDirStr(FileUtil<wchar_t>::ParentDir);
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
	WIN32_FIND_DATAW findFileData;
	HANDLE h = FindFirstFileW(join(dir,L"*.*").c_str(), &findFileData);
	if(h == INVALID_HANDLE_VALUE){
		return;
	}
	do{
		std::wstring name(join(dir,findFileData.cFileName));
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if(FileUtil<std::wstring>::CurrentDirStr!=findFileData.cFileName && FileUtil<std::wstring>::ParentDirStr!=findFileData.cFileName && recursive){
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
	struct dirent* de;
	DIR* d = opendir(dir.c_str());
	while((de = readdir(d)) != nullptr){
		std::string name = join(dir,Sep,de->d_name);
		struct stat64 st;
		if(lstat64(name.c_str(), &st) != 0){
			throw logging::Exception(__FILE__, __LINE__, "Failed to lstat64.");
		}
		if(S_IFDIR & st.st_mode){
			if(FileUtil<std::string>::CurrentDirStr!=de->d_name && FileUtil<std::string>::ParentDirStr != de->d_name && recursive){
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
