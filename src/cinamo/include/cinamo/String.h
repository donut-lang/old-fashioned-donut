/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cstdarg>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "Common.h"
#include "Platform.h"

#pragma once
namespace cinamo {

std::string format(std::string const& fmt, ...);
std::string formatv(std::string const& fmt, va_list args);

/**
 * toString 各種プリミティブをstringに変換する。
 */
#define TOSTR_DEF(type) std::string toString(type const& val, int radix=10);
TOSTR_DEF(char);
TOSTR_DEF(unsigned char);
TOSTR_DEF(signed char);
TOSTR_DEF(int);
TOSTR_DEF(unsigned int);
TOSTR_DEF(long int);
TOSTR_DEF(unsigned long int);
TOSTR_DEF(long long int);
TOSTR_DEF(unsigned long long int);
#undef TOSTR_DEF
std::string toString(const float val);
std::string toString(const double val);
std::string toString(const bool val);
inline std::string toString(std::string const& s) { return s; };

/**
 * 小文字の文字列に変換する
 */
std::string toLower(std::string const& str);
/**
 * 大文字の文字列に変換する
 */
std::string toUpper(std::string const& str);

std::string join(std::vector<std::string> const& lst);
std::string join(std::vector<std::string> const& lst, std::string const& sep);
std::size_t matchString(std::string const& a, std::string const& b);
std::string randomString(std::size_t len);

#if defined(HAVE_ICU) && HAVE_ICU
std::vector<std::string> breakChar(std::string const& str_);
std::string trim(std::string const& str);
std::string& trim(std::string& str);
#endif

template <typename T> bool tryParseAs(std::string const& str, int radix, T& result);
template <typename T> bool tryParseAs(std::string const& str, T& result);
template <typename T> T parseAs(std::string const& str, int radix, T const& default_);
template <typename T> T parseAs(std::string const& str, T const& default_);

std::vector<std::string> split(std::string const& str, std::string const& sep);
template <size_t N> inline std::vector<std::string> split(std::string const& str, const std::string (&sep)[N]);
std::vector<std::string> splitSpace(std::string const& str);
std::vector<std::string> splitLine(std::string const& str);

bool startsWith(std::string const& target, std::string const& prefix);
bool endsWith(std::string const& target, std::string const& suffix);

}

#include "internal/String.h"
