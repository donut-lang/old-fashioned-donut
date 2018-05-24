/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <tinyxml2.h>
#include "Common.h"

#define DEF_OVERLOD(Klass)\
template <> void parseAttr(std::string const& name, Klass& ptr, Klass const& def, tinyxml2::XMLElement* elm)

namespace cinamo {
namespace xml {

template <typename T> void parseAttr(std::string const& name, T& ptr, const T& def, tinyxml2::XMLElement* elm);
DEF_OVERLOD(std::string);
DEF_OVERLOD(float);
DEF_OVERLOD(int);
DEF_OVERLOD(unsigned int);
DEF_OVERLOD(unsigned long long);
DEF_OVERLOD(long long);
DEF_OVERLOD(bool);

}}

#undef DEF_OVERLOD
