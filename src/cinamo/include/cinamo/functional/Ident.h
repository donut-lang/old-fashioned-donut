/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, psi
 */
#pragma once

namespace cinamo {

template <typename A>
class Ident {
public:
	using type = A;
};

static_assert(std::is_same<int, typename Ident<int>::type >::value, "oops");

}
