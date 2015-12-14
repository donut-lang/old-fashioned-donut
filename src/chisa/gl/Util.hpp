/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

namespace chisa {
namespace gl {

constexpr int getPower2Of(const int width, const int acc=1) noexcept
{
	return acc >= width ? acc : getPower2Of(width, acc<<1);
}

}}
