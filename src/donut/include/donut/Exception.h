/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <exception>
#include <string>
#include <cstdarg>

namespace donut {

class DonutException final : public std::exception {
private:
	std::string _loc;
	std::string _msg;
	std::string _file;
	std::string _what;
	size_t _line;
protected:
	void init(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
	void init(const char* file, const size_t line, std::string const& fmt, ...) noexcept;
public:
	DonutException(const char* file, const size_t line) noexcept;
	DonutException(const char* file, const size_t line, std::string const& fmt, ...) noexcept;
	DonutException(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
	virtual ~DonutException() noexcept = default;
	virtual const char* what() const noexcept override;
	std::string msg() const noexcept;
	std::string file() const noexcept;
	size_t line() const noexcept;
};

#define DONUT_EXCEPTION(Klass, ...) throw Donut##Klass(__FILE__, __LINE__, __VA_ARGS__)

}

