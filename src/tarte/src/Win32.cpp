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

#include <tarte/Platform.h>
#if CHISA_WINDOWS
#include <tarte/internal/Win32.h>
#include <tarte/Logger.h>
#include <tarte/Exception.h>

namespace tarte {
namespace internal {
namespace win32 {

std::wstring toUTF16(std::string const& str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);
	wchar_t* buf = new wchar_t[size+1];
	if(size != MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1)){
		TARTE_EXCEPTION(Exception, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::wstring ret(buf);
	delete [] buf;
	return ret;
}
std::string toUTF8(std::wstring const& str)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0, nullptr, nullptr);
	char* buf = new char[size+1];
	if(size != WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), buf, size+1, nullptr, nullptr)){
		TARTE_EXCEPTION(Exception, "Failed to convert UTF8 to UTF16");
	}
	buf[size]=0;
	std::string ret(buf);
	delete [] buf;
	return ret;
}

}}}

#endif
