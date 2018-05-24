/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <cinamo/Exception.h>

namespace cinamo {

Exception::Exception(const char* file, const size_t line) throw()
:loc_()
,msg_()
,file_()
,what_()
,line_(-1)
{
	init(file, line, "");
}

Exception::Exception(const char* file, const size_t line, std::string const& fmt, ...) noexcept
:loc_()
,msg_()
,file_()
,what_()
,line_(-1)
{
	va_list lst;
	va_start(lst, fmt);
	init(file, line, fmt, lst);
	va_end(lst);
}

Exception::Exception(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept
:loc_()
,msg_()
,file_()
,what_()
,line_(-1)
{
	init(file, line, fmt, lst);
}

void Exception::init(const char* file, const size_t line, std::string const& fmt, ...) noexcept
{
	va_list lst;
	va_start(lst, fmt);
	init(file, line, fmt, lst);
	va_end(lst);
}

void Exception::init(const char* file, const size_t line, std::string const& fmt, va_list lst) noexcept
{
	try{
		this->line_ = line;
		this->file_ = std::string(file);
		this->loc_ = format("(in %s:%d): ", file, line);
		this->msg_ = formatv(fmt, lst);
		this->what_ = loc_+msg_;
	}catch(...){
		try {
			this->line_ = 0;
			this->file_ = __FILE__;
			this->loc_ = "line ? in " __FILE__ ": ";
			this->msg_ = "[BUG] Failed to format string!!";
			this->what_ = loc_+msg_;
		}catch(...){}
	}
}

const char* Exception::what() const noexcept (true)
{
	return this->what_.c_str();
}

std::string Exception::msg() const noexcept
{
	return this->msg_;
}
size_t Exception::line() const noexcept
{
	return this->line_;
}

std::string Exception::file() const noexcept
{
	return this->file_;
}

}
