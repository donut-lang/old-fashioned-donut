/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <string>
#include "Buffer.hpp"

namespace chisa {
namespace gl{
namespace internal {
static const std::string TAG("Buffer");

Buffer::Buffer(std::size_t size)
:size_(size), ptr_(new unsigned char [size])
{

}
Buffer::~Buffer() noexcept
{
	delete [] ptr_;
}


}}}
