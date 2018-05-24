/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Dynamic.h>

#include <cxxabi.h>

namespace cinamo {


std::string demangle(std::type_info const& info)
{
	int status;
	return abi::__cxa_demangle(info.name(), 0, 0, &status);
}

}
