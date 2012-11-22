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
#include "../../../TestCommon.h"
#include "../../../../src/chisa/donut/parser/Parser.h"
#include "../../../../src/chisa/donut/object/Heap.h"
#include "../../../../src/chisa/donut/source/Source.h"
#include "../../../../src/chisa/donut/Clock.h"
#include "../../../../src/chisa/donut/vm/Machine.h"

#define INIT_DONUT\
	Handler<Clock> clock( new Clock() );\
	Handler<Heap> heap(new Heap(log_trace, clock));\
	heap->bootstrap();\
	Machine machine(log_trace, clock, heap);

#define EXECUTE_SRC(src) INIT_DONUT\
	Handler<Source> source = Parser::fromString(src, "<MEM>", 0)->parseProgram();\
	Handler<Object> result = machine.start(source);

#define SOURCE_TEST_THROW(src) INIT_DONUT\
{\
	Handler<Source> source = Parser::fromString(src, "<MEM>", 0)->parseProgram();\
	ASSERT_ANY_THROW( machine.start( source ) );\
}

#define SOURCE_TEST_NO_THROW(src) INIT_DONUT\
{\
	Handler<Source> source = Parser::fromString(src, "<MEM>", 0)->parseProgram();\
	ASSERT_NO_THROW( machine.start( source ) );\
}

#define SOURCE_TEST_TRUE(expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_TRUE(result->toBool(heap));\
}

#define SOURCE_TEST_FALSE(expr)\
{\
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isObject());\
	ASSERT_FALSE(result->isInt());\
	ASSERT_TRUE(result->isBool());\
	ASSERT_FALSE(result->toBool(heap));\
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
	EXECUTE_SRC(expr);\
	ASSERT_FALSE(result->isNull());\
	ASSERT_FALSE(result->isBool());\
	ASSERT_TRUE(result->isObject());\
	ASSERT_FALSE(result->isInt());\
	ASSERT_EQ(std::string(str), result->toString(heap));\
}
