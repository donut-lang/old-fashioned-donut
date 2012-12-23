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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <tarte/Logger.h>
#include <tarte/Exception.h>
#include <tarte/String.h>

namespace tarte {

Logger::Logger(std::ostream& stream, enum Level level) noexcept
:_stream(stream), level(level)
{
}

void Logger::msg(enum Level level, std::string const& tag, std::string const& fmt, std::va_list args)
{
	std::stringstream ss;
	if(level < this->level){
		return;
	}
	switch(level){
	case TRACE_:
		ss << "[T]";
		break;
	case VERBOSE_:
		ss << "[V]";
		break;
	case DEBUG_:
		ss << "[D]";
		break;
	case INFO_:
		ss << "[I]";
		break;
	case WARN_:
		ss << "[W]";
		break;
	case ERROR_:
		ss << "[E]";
		break;
	default:
		throw Exception(__FILE__, __LINE__, "[BUG][FIXME] Invalid log level!!");
	}
	ss << "[" << std::setw(16) << tag << "] ";
	ss << formatv(fmt, args) << std::endl;
	_stream << ss.str();
	_stream.flags();
}

bool Logger::t() const noexcept
{
	return this->level <= TRACE_;
}
void Logger::t(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(TRACE_, tag, fmt, lst);
	va_end(lst);

}
bool Logger::v() const noexcept
{
	return this->level <= VERBOSE_;
}

void Logger::v(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(VERBOSE_, tag, fmt, lst);
	va_end(lst);
}
bool Logger::d() const noexcept
{
	return this->level <= DEBUG_;
}
void Logger::d(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(DEBUG_, tag, fmt, lst);
	va_end(lst);
}
bool Logger::i() const noexcept
{
	return this->level <= INFO_;
}
void Logger::i(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(INFO_, tag, fmt, lst);
	va_end(lst);
}
void Logger::w(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(WARN_, tag, fmt, lst);
	va_end(lst);
}
void Logger::e(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	msg(ERROR_, tag, fmt, lst);
	va_end(lst);
}

}
