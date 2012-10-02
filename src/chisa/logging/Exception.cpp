/**
 * Saccubus
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

#include "../util/StringUtil.h"
#include "Exception.h"

namespace chisa {
namespace logging {

Exception::Exception(const char* file, const size_t line) throw() : _line(0)
{
}

Exception::Exception(const char* file, const size_t line, const std::string& fmt, ...) throw()
{
	va_list lst;
	va_start(lst, fmt);
	init(file, line, fmt, lst);
	va_end(lst);
}

Exception::Exception(const char* file, const size_t line, const std::string& fmt, va_list lst) throw()
{
	init(file, line, fmt, lst);
}

void Exception::init(const char* file, const size_t line, const std::string& fmt, va_list lst) throw()
{
	try{
		this->_line = line;
		this->_file = std::string(file);
		this->_loc = util::format("(in %s:%d): ", file, line);
		this->_msg = util::formatv(fmt, lst);
	}catch(...){
		this->_line = 0;
		this->_file = __FILE__;
		this->_loc = "line ? in " __FILE__ ": ";
		this->_msg = "[BUG] Failed to format string!!";
	}
}

Exception::~Exception() throw(){
}

std::string Exception::what()
{
	return _loc+_msg;
}

std::string Exception::msg()
{
	return _msg;
}
size_t Exception::line()
{
	return _line;
}


}}
