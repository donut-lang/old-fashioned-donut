/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/native/Encoder.h>

#include <donut/object/Heap.h>
#include <donut/object/StringObject.h>
#include <donut/object/FloatObject.h>

namespace donut {
namespace native {

#define INT_ENCODER(TYPE)\
template <>\
Handler<Object> encode<TYPE>(Handler<Heap> const& heap, TYPE val)\
{\
	return heap->createInt(val);\
}
INT_ENCODER(long long);
INT_ENCODER(unsigned long long);
INT_ENCODER(long);
INT_ENCODER(unsigned long);
INT_ENCODER(int);
INT_ENCODER(unsigned int);
INT_ENCODER(short);
INT_ENCODER(unsigned short);
INT_ENCODER(char);
INT_ENCODER(unsigned char);
INT_ENCODER(signed char);

#define FLOAT_ENCODER(TYPE)\
template <>\
Handler<Object> encode<TYPE>(Handler<Heap> const& heap, TYPE val)\
{\
	return heap->createFloatObject(val);\
}

FLOAT_ENCODER(float);
FLOAT_ENCODER(double);
FLOAT_ENCODER(long double);

template <>
Handler<Object> encode<bool>(Handler<Heap> const& heap, bool val)
{
	return heap->createBool(val);
}

template <>
Handler<Object> encode<std::string>(Handler<Heap> const& heap, std::string val)
{
	return heap->createStringObject(val);
}
template <>
Handler<Object> encode<const char*>(Handler<Heap> const& heap, const char* val)
{
	return heap->createStringObject(val);
}

template <>
Handler<Object> encode<std::nullptr_t>(Handler<Heap> const& heap, std::nullptr_t)
{
	return heap->createNull();
}

}}
