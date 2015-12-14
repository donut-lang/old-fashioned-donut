/**
 * Saccubus
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

#include "../../TestCommon.hpp"
#include <cinamo/Logger.h>
#include <sstream>

namespace chisa {
namespace logging {

TEST(LoggerTest, LevelTest)
{
	std::ostringstream ss;
	Logger log1(ss, Logger::TRACE_);
	Logger log2(ss, Logger::VERBOSE_);
	Logger log3(ss, Logger::DEBUG_);
	Logger log4(ss, Logger::INFO_);
	Logger log5(ss, Logger::WARN_);
	Logger log6(ss, Logger::ERROR_);

	ASSERT_TRUE(log1.t());
	ASSERT_TRUE(log1.v());
	ASSERT_TRUE(log1.d());
	ASSERT_TRUE(log1.i());


	ASSERT_FALSE(log2.t());
	ASSERT_TRUE(log2.v());
	ASSERT_TRUE(log2.d());
	ASSERT_TRUE(log2.i());

	ASSERT_FALSE(log3.t());
	ASSERT_FALSE(log3.v());
	ASSERT_TRUE(log3.d());
	ASSERT_TRUE(log3.i());

	ASSERT_FALSE(log4.t());
	ASSERT_FALSE(log4.v());
	ASSERT_FALSE(log4.d());
	ASSERT_TRUE(log4.i());

	ASSERT_FALSE(log5.t());
	ASSERT_FALSE(log5.v());
	ASSERT_FALSE(log5.d());
	ASSERT_FALSE(log5.i());

	ASSERT_FALSE(log6.t());
	ASSERT_FALSE(log6.v());
	ASSERT_FALSE(log6.d());
	ASSERT_FALSE(log6.i());
}
TEST(LoggerTest, LogTest)
{
	std::ostringstream ss;
	Logger log(ss, Logger::DEBUG_);
	log.t("TestTag", "Test: %d", 12);
	ASSERT_EQ(0U, ss.str().size());
	log.d("TestTag", "Test: %d", 12);
	ASSERT_GT(ss.str().size(), 0U);
}

}}
