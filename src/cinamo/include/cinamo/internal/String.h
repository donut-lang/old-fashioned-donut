/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <string>

#pragma once
namespace cinamo {

template <typename T>
T parseAs(std::string const& str, int radix, T const& default_)
{
	T result;
	if( tryParseAs<T>(str, radix, result) ){
		return result;
	}else{
		return default_;
	}
}
template <typename T>
T parseAs(std::string const& str, T const& default_)
{
	T result;
	if( tryParseAs<T>(str, result) ){
		return result;
	}else{
		return default_;
	}
}

namespace internal {
std::vector<std::string> split(std::string const& str, const std::string* sep, size_t n);
}

template <size_t N>
std::vector<std::string> inline split(std::string const& str, const std::string (&sep)[N])
{
	return internal::split(str, sep, N);
}

}
