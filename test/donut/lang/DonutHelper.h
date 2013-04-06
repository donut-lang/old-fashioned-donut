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
#include <cinamo/String.h>
#include "../../TestCommon.h"
#include <donut/Donut.h>
#include <donut/object/FloatObject.h>
#include <donut/object/StringObject.h>

namespace donut {
using namespace cinamo;
}

#define INIT_DONUT\
	Handler<Donut> donut(new Donut(log_trace));\
	donut->bootstrap();\
	Handler<Heap> heap( donut->heap() );\
	Handler<Machine> machine = donut->queryMachine();

#define INIT_DONUT_WITH(SAVE_DATA)\
	Handler<Donut> donut(new Donut(log_trace));\
	donut->load(SAVE_DATA);\
	Handler<Heap> heap( donut->heap() );\
	Handler<Machine> machine = donut->queryMachine();

#define EXECUTE_SRC(src) INIT_DONUT\
	Handler<Object> result = machine->start( donut->parse(src, __FILE__, __LINE__) );

#define SOURCE_TEST_THROW(src) INIT_DONUT\
{\
	ASSERT_ANY_THROW( machine->start( donut->parse(src, "<MEM>", 0) ) );\
}

#define SOURCE_TEST_NO_THROW(src) INIT_DONUT\
{\
	Handler<Source> source = donut->parse(src, "<MEM>", 0);\
	ASSERT_NO_THROW( machine->start( source ) );\
}

#define SOURCE_TEST_TRUE(expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_TRUE( result->toBool(heap) );\
}

#define SOURCE_TEST_FALSE(expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_FALSE( result->toBool(heap) );\
}

#define SOURCE_TEST_NULL(expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_TRUE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_FALSE(result->isInt());\
	ASSERT_FALSE(result->isBool());\
}

#define SOURCE_TEST_INT(num, expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isBool());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isInt());\
	ASSERT_EQ(num, result->toInt(heap));\
}

#define SOURCE_TEST_FLOAT(num, expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isBool());\
	ASSERT_TRUE(result->isObject());\
	ASSERT_FALSE(result->isInt());\
	ASSERT_FLOAT_EQ(num, result->toFloat(heap));\
}

#define SOURCE_TEST_STR(str, expr)\
{\
	std::string val;\
	EXECUTE_SRC(expr);\
	{\
		ASSERT_FALSE(result->isNull());\
		ASSERT_FALSE(result->isBool());\
		ASSERT_TRUE(result->isObject());\
		ASSERT_FALSE(result->isInt());\
		ASSERT_NO_THROW(val=result->toString(heap));\
		ASSERT_EQ(std::string(str), val);\
	}\
}
