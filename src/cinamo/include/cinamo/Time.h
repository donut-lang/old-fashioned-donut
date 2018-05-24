/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "Common.h"
#include "Platform.h"

namespace cinamo {

#if defined(HAVE_ULONGLONG)
unsigned long long getTimeInMills();
#else
unsigned long getTimeInMills();
#endif

}
