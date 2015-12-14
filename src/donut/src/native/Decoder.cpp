/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/native/Decoder.hpp>

namespace donut {
namespace native {

#define INT_DECODER(TYPE)\
template <>\
TYPE decode<TYPE>(Handler<Heap> const& heap, Handler<Object> obj)\
{\
	return obj->toInt(heap);\
}
INT_DECODER(long long);
INT_DECODER(unsigned long long);
INT_DECODER(long);
INT_DECODER(unsigned long);
INT_DECODER(int);
INT_DECODER(unsigned int);
INT_DECODER(short);
INT_DECODER(unsigned short);
INT_DECODER(char);
INT_DECODER(unsigned char);
INT_DECODER(signed char);

#define FLOAT_DECODER(TYPE)\
template <>\
TYPE decode<TYPE>(Handler<Heap> const& heap, Handler<Object> obj)\
{\
	return obj->toFloat(heap);\
}

FLOAT_DECODER(float);
FLOAT_DECODER(double);
FLOAT_DECODER(long double);

template <>
bool decode<bool>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toBool(heap);
}

template <>
std::string decode<std::string>(Handler<Heap> const& heap, Handler<Object> obj)
{
	return obj->toString(heap);
}

}}
