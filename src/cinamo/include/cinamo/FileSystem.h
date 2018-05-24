/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <string>
#include <vector>
#include "Common.h"
#include "Platform.h"
#include "internal/FileSystem.h"

namespace cinamo {
namespace file {

template <typename T, typename... Args>
typename internal::FileConstants<T>::string_type join(const T& a, Args const&... left)
{
	return typename internal::FileConstants<T>::string_type(a)+internal::FileConstants<T>::Sep+join(left...);
}

std::vector<std::string> enumFiles(std::string const& dir, bool recursive=false);
std::string readAsString(std::istream& stream);
std::string readAsString(std::string const& fname);

}}
