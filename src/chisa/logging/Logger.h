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

#include <ostream>
#include <cstdarg>
#include <string>

namespace chisa {
namespace logging{
/**
 * FIXME: さきゅばすから取ってきたそのまんま
 */

/**
 * ロガー。
 *
 * ログを書き出す先を抽象化して、
 * ログレベル管理やプリティプリントなどの
 * ユーティリティを付加したクラス。
 */
class Logger {
public:
	enum Level{
		TRACE_=0,
		VERBOSE_=1,
		DEBUG_=2,
		INFO_=3,
		WARN_=4,
		ERROR_=5,
	};
private:
	std::ostream& _stream;
	enum Level level;
	void msg(enum Level level, const std::string& tag, const std::string& fmt, std::va_list args);
public:
	Logger(std::ostream& stream, enum Level level);
	virtual ~Logger();
public:
	std::ostream& stream() const{ return _stream; };
public:
	bool t();
	void t(const std::string& tag, const std::string& fmt, ...);
	bool v();
	void v(const std::string& tag, const std::string& fmt, ...);
	bool d();
	void d(const std::string& tag, const std::string& fmt, ...);
	bool i();
	void i(const std::string& tag, const std::string& fmt, ...);
	void w(const std::string& tag, const std::string& fmt, ...);
	void e(const std::string& tag, const std::string& fmt, ...);
};

}}

