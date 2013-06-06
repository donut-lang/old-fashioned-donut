/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>

namespace chisa {
namespace gl {
namespace internal {

class Buffer final{
	DISABLE_COPY_AND_ASSIGN (Buffer);
	DEFINE_MEMBER_CONST(public, std::size_t, size);
	DEFINE_MEMBER_CONST(public, unsigned char*, ptr);
public:
	Buffer(std::size_t size);
	~Buffer() noexcept;
};

}}}
