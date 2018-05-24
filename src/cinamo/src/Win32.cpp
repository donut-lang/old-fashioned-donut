/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Platform.h>
#if CINAMO_WINDOWS
#include <cinamo/internal/Win32.h>
#include <cinamo/Logger.h>
#include <cinamo/Exception.h>

namespace cinamo {
namespace internal {
namespace win32 {

std::string toSystem(std::string const& str)
{
	std::wstring const utf16(toUTF16(str));
	const int size = WideCharToMultiByte(CP_THREAD_ACP, 0, utf16.c_str(), utf16.length(), nullptr, 0, nullptr, nullptr);
	char* const buf = new char[size+1];
	if(size != WideCharToMultiByte(CP_THREAD_ACP, 0, utf16.c_str(), utf16.length(), buf, size+1, nullptr, nullptr)){
		CINAMO_EXCEPTION(Exception, "Failed to convert UTF16 to System code page!");
	}
	buf[size]=0;
	std::string const ret(buf);
	delete [] buf;
	return ret;

}

std::wstring toUTF16(std::string const& str)
{
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);
	wchar_t* const buf = new wchar_t[size+1];
	if(size != MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1)){
		CINAMO_EXCEPTION(Exception, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::wstring const ret(buf);
	delete [] buf;
	return ret;
}
std::string toUTF8(std::wstring const& str)
{
	const int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0, nullptr, nullptr);
	char* const buf = new char[size+1];
	if(size != WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1, nullptr, nullptr)){
		CINAMO_EXCEPTION(Exception, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::string const ret(buf);
	delete [] buf;
	return ret;
}

}}}

#endif
