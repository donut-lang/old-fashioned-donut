/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ProviderDecl.h"

namespace donut {

class StringObject;
class StringProvider final : public HeapProviderBaseT<StringProvider, StringObject> {
public:
	StringProvider(Handler<Heap> const& heap);
	virtual ~StringProvider() noexcept = default;
};

}
