/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ProviderDecl.h"

namespace donut {

class FloatObject;
class FloatProvider final : public HeapProviderBaseT<FloatProvider, FloatObject> {
public:
	FloatProvider(Handler<Heap> const& heap);
	virtual ~FloatProvider() noexcept = default;
};

}
