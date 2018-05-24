/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Platform.h"
#if CINAMO_WINDOWS
#include <Windows.h>
#include <string>

namespace cinamo {
namespace internal {
namespace win32 {

std::wstring toUTF16(std::string const& str);
std::string toUTF8(std::wstring const& str);
std::string toSystem(std::string const& str);

}}}

#endif
