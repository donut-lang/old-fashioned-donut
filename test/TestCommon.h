/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>
#include <iostream>

#include <tarte/Exception.h>
#include <tarte/Logger.h>

#include <gtest/gtest.h>
#include <tinyxml2.h>


using namespace tarte;

#define MATERIAL_DIR "__testfiles__"

#define NULL_STREAM (*((std::ostream*)0))
static Logger log_trace(std::cout, Logger::TRACE_);
static Logger log_err(std::cout, Logger::ERROR_);

std::shared_ptr<tinyxml2::XMLDocument> parse(std::string const& str);

template <int... Args>
void static_debug(){
	static_assert(sizeof...(Args) < 0, "see type for compiler");
}

template <bool... Args>
void static_debug(){
	static_assert(sizeof...(Args) < 0, "see type for compiler");
}

template <typename... Args>
void static_debug(){
	static_assert(sizeof...(Args) < 0, "see type for compiler");
}

template<typename T, size_t N>
::testing::AssertionResult ArraysMatch(const T (&expected)[N], const T (&actual)[N])
{
	for (size_t i(0); i < N; ++i) {
		if (expected[i] != actual[i]) {
			return ::testing::AssertionFailure() << "array[" << i << "] ( = \"" << actual[i] << "\") != expected[" << i << "] ( = \"" << expected[i] << "\")";
		}
	}
	return ::testing::AssertionSuccess();
}
