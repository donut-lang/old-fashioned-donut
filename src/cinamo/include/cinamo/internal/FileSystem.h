/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Platform.h"
#include <string>
#include <set>

namespace cinamo {
namespace file {

namespace internal {

template <class String>
struct FileConstants ;

#if CINAMO_WINDOWS
#define CINAMO_FILE_SEP "\\"
#define CINAMO_FILE_SEPW L"\\"
#else
#define CINAMO_FILE_SEP "/"
#define CINAMO_FILE_SEPW L"/"
#endif

#define STRING_INNER \
typedef std::string string_type;\
static constexpr const char* Sep = CINAMO_FILE_SEP;\
static constexpr const char* CurrentDir = ".";\
static constexpr const char* ParentDir = "..";

#define WSTRING_INNER \
typedef std::wstring string_type;\
static constexpr const wchar_t* Sep = CINAMO_FILE_SEPW;\
static constexpr const wchar_t* CurrentDir = L".";\
static constexpr const wchar_t* ParentDir = L"..";

template<> struct FileConstants<std::string>{ STRING_INNER };
template<> struct FileConstants<std::wstring> { WSTRING_INNER };

template<> struct FileConstants<char> { STRING_INNER };
template<> struct FileConstants<wchar_t> { WSTRING_INNER };

template<> struct FileConstants<char*> { STRING_INNER };
template<> struct FileConstants<wchar_t*> { WSTRING_INNER };

template<> struct FileConstants<const char*> { STRING_INNER };
template<> struct FileConstants<const wchar_t*> { WSTRING_INNER };

template<std::size_t N> struct FileConstants<char[N]> { STRING_INNER };
template<std::size_t N> struct FileConstants<wchar_t [N]> { WSTRING_INNER };

#undef CINAMO_FILE_SEP
#undef CINAMO_FILE_SEPW
#undef STRING_INNER
#undef WSTRING_INNER

}

template <typename T>
const T& join(const T& a)
{
	return a;
}

}}
