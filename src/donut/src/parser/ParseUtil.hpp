/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <antlr3.h>
#include <math.h>

namespace donut{

const std::string createStringFromString(pANTLR3_STRING string);
const std::string createStringFromToken(pANTLR3_COMMON_TOKEN tok);
const std::string unescapeString(std::string const& str);

}
