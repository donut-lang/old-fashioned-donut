/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cinamo/Logger.h>
#include <cinamo/Exception.h>
#include <cinamo/String.h>

#include <cinamo/internal/Win32.h>

namespace cinamo {

struct null_stream_sink_t : std::streambuf {
	int overflow(int c){return c;}
};
static null_stream_sink_t null_stream_sink;
std::ostream null_stream(&null_stream_sink);

Logger::Logger(std::ostream& stream, enum Level level) noexcept
:_stream(stream), level(level)
{
}

bool Logger::msg(enum Level level, std::string const& tag, std::string const& fmt, std::va_list args)
{
	std::stringstream ss;
	if(level < this->level){
		return false;
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
#if CINAMO_WINDOWS
	ss << ::cinamo::internal::win32::toSystem(formatv(fmt, args)) << std::endl;
	//ss << formatv(fmt, args) << std::endl;
#else
	ss << formatv(fmt, args) << std::endl;
#endif
	_stream << ss.str();
	_stream.flags();
	return true;
}

bool Logger::t(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(TRACE_, tag, fmt, lst);
	va_end(lst);
	return res;
}

bool Logger::v(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(VERBOSE_, tag, fmt, lst);
	va_end(lst);
	return res;
}

bool Logger::d(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(DEBUG_, tag, fmt, lst);
	va_end(lst);
	return res;
}

bool Logger::i(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(INFO_, tag, fmt, lst);
	va_end(lst);
	return res;
}
bool Logger::w(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(WARN_, tag, fmt, lst);
	va_end(lst);
	return res;
}
bool Logger::e(std::string const& tag, std::string const& fmt, ...)
{
	std::va_list lst;
	va_start(lst, fmt);
	bool const res = msg(ERROR_, tag, fmt, lst);
	va_end(lst);
	return res;
}

}
