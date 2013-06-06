/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "Provider.h"

namespace donut {

class HomuraObject;
class HomuraProvider final : public HeapProviderBaseT<HomuraProvider, HomuraObject> {
public:
	HomuraProvider(Handler<Heap> const& heap);
	virtual ~HomuraProvider() noexcept = default;
};

}
