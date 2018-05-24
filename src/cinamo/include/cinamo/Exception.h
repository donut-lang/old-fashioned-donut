/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <exception>
#include <string>
#include <cstdarg>
#include "Common.h"
#include "ClassUtil.h"

namespace cinamo {

class Exception : public std::exception {
private:
	std::string loc_;
	std::string msg_;
	std::string file_;
	std::string what_;
	size_t line_;
private:
	void init(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
	void init(const char* file, const size_t line, std::string const& fmt, ...) noexcept;
public:
	Exception(const char* file, const size_t line) noexcept;
	Exception(const char* file, const size_t line, std::string const& fmt, ...) noexcept;
	Exception(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept;
	virtual ~Exception() noexcept override = default;
	virtual const char* what() const noexcept override;
	std::string msg() const noexcept;
	std::string file() const noexcept;
	size_t line() const noexcept;
};

#define CINAMO_EXCEPTION(Klass, ...) throw ::cinamo::Klass(__FILE__, __LINE__, __VA_ARGS__);

}
