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

#pragma once
#include <exception>
#include <string>
#include <cstdarg>

namespace tarte {

class Exception : public std::exception {
private:
	std::string _loc;
	std::string _msg;
	std::string _file;
	std::string _what;
	size_t _line;
protected:
	void init(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
public:
	Exception(const char* file, const size_t line) noexcept;
	Exception(const char* file, const size_t line, std::string const& fmt, ...) noexcept;
	Exception(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
	virtual ~Exception() noexcept = default;
	virtual const char* what() const noexcept override;
	std::string msg() const noexcept;
	std::string file() const noexcept;
	size_t line() const noexcept;
};

#define TARTE_EXCEPTION(Klass, ...) throw Klass(__FILE__, __LINE__, __VA_ARGS__);

}
