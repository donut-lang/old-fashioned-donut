/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../geom/Decl.h"

namespace chisa {
namespace gl {
class Canvas;
class Color;

void renderCubicLine(gl::Canvas& cv, float width, gl::Color const& color, geom::Point const& pt1, geom::Point const& pt2);

}}
