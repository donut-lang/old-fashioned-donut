/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/FileSystem.h>
#include <cinamo/Exception.h>
#if CINAMO_WINDOWS
#include <cinamo/internal/Win32.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <sstream>
#include <fstream>

namespace cinamo {
namespace file {
namespace internal {

}

std::string readAsString(std::istream& stream)
{
	std::stringstream source_;
	std::string buf;
	while(stream && std::getline(stream, buf)){
		while(buf.size() > 0 && (buf.at(buf.size()-1)=='\r' || buf.at(buf.size()-1)=='\n')){
			buf=buf.substr(0, buf.size()-1);
		}
		while(buf.size() > 0 && (buf.at(0)=='\r' || buf.at(0)=='\n')){
			buf=buf.substr(1);
		}
		source_ << buf << std::endl;
	}
	return source_.str();
}

std::string readAsString(std::string const& fname)
{
	std::ifstream stream(fname);
	return readAsString(stream);
}

#if CINAMO_WINDOWS
static void enumFilesImpl(std::wstring const& dir, std::vector<std::string>& list, bool recursive)
{
	using namespace internal;
	using namespace ::cinamo::internal::win32;
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
	using namespace ::cinamo::internal::win32;
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
		CINAMO_EXCEPTION(Exception, "[BUG] Failed to open dir: %s", dir.c_str());
	}
	while((de = readdir(d)) != nullptr){
		std::string name = join(dir, de->d_name);
		struct stat64 st;
		if(lstat64(name.c_str(), &st) != 0){
			CINAMO_EXCEPTION(Exception, "Failed to lstat64.");
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
