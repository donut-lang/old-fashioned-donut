/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "Common.h"
#include <cfloat>
#include <climits>
//#include <cinamo/Config.h>

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#define CINAMO_IOS
#endif

#ifdef __APPLE__
#define CINAMO_OSX
#endif

#if defined(__linux__) || defined(__linux) || defined(__gnu_linux__) || defined(linux)
#define CINAMO_LINUX
#endif

#if defined(WIN32) || defined(WIN64) || defined(__WIN32__) || defined(__WIN64__)
#define CINAMO_WINDOWS
#endif

#ifdef __ANDROID__
#define CINAMO_ANDROID
#endif

#ifdef FLT_MAX
#define HAVE_FLOAT
#endif

#ifdef DBL_MAX
#define HAVE_DOUBLE
#endif

#ifdef LDBL_MAX
#define HAVE_LONG_DOUBLE
#endif

#if defined(HAVE_LONG_DOUBLE) && !CINAMO_ANDROID
#define HAVE_STRTOLD
#endif

#ifdef CHAR_MAX
#define HAVE_CHAR
#endif

#ifdef UCHAR_MAX
#define HAVE_UCHAR
#endif

#ifdef SCHAR_MAX
#define HAVE_SCHAR
#endif

#ifdef SHRT_MAX
#define HAVE_SHORT
#endif

#ifdef USHRT_MAX
#define HAVE_USHORT
#endif

#ifdef LONG_MAX
#define HAVE_LONG
#endif

#ifdef ULONG_MAX
#define HAVE_ULONG
#endif

#define HAVE_LONGLONG (defined(LLONG_MAX))
#define HAVE_ULONGLONG (defined(ULLONG_MAX))
#define HAVE_STRTOLL (HAVE_LONGLONG)
#define HAVE_STRTOULL (HAVE_ULONGLONG)

#if defined(CINAMO_WINDOWS)
#define IS_BIG_ENDIAN 0
#elif defined(CINAMO_OSX)
#define IS_BIG_ENDIAN 0
#else
namespace {
#include <endian.h>
#define IS_BIG_ENDIAN (BYTE_ORDER == BIG_ENDIAN)
}
#endif

