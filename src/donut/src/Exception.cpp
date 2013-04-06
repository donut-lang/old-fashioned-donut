/**
 * Donut
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

#include <cinamo/String.h>
#include <donut/Exception.h>

namespace donut {

DonutException::DonutException(const char* file, const size_t line) throw()
{
	init(file, line, "");
}

DonutException::DonutException(const char* file, const size_t line, std::string const& fmt, ...) noexcept
{
	va_list lst;
	va_start(lst, fmt);
	init(file, line, fmt, lst);
	va_end(lst);
}

DonutException::DonutException(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept
{
	init(file, line, fmt, lst);
}

void DonutException::init(const char* file, const size_t line, std::string const& fmt, ...) noexcept
{
	va_list lst;
	va_start(lst, fmt);
	init(file, line, fmt, lst);
	va_end(lst);
}

void DonutException::init(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept
{
	try{
		this->_line = line;
		this->_file = std::string(file);
		this->_loc = ::cinamo::format("(in %s:%d): ", file, line);
		this->_msg = ::cinamo::formatv(fmt, lst);
		this->_what = _loc+_msg;
	}catch(...){
		this->_line = 0;
		this->_file = __FILE__;
		this->_loc = "line ? in " __FILE__ ": ";
		this->_msg = "[BUG] Failed to format string!!";
		this->_what = _loc+_msg;
	}
}

const char* DonutException::what() const noexcept (true)
{
	return _what.c_str();
}

std::string DonutException::msg() const noexcept
{
	return _msg;
}
size_t DonutException::line() const noexcept
{
	return _line;
}

std::string DonutException::file() const noexcept
{
	return _file;
}

}
