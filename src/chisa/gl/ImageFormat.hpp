/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "OpenGL.hpp"

namespace chisa {
using namespace cinamo;
namespace gl {
class Canvas;
enum class ImageFormat : unsigned int {
	Invalid = 0,
	RGBA8 = GL_RGBA,
	BGRA8 = GL_BGRA,
	ALPHA = GL_ALPHA
};

}}
