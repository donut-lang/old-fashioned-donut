/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ProviderDecl.h"

namespace donut {

class DonutObject;
class DonutClosureObject;
class DonutProvider final : public HeapProviderBaseT<DonutProvider, DonutObject> {
public:
	DonutProvider( Handler<Heap> const& heap );
	virtual ~DonutProvider() noexcept = default;
};
class DonutClosureProvider final : public HeapProviderBaseT<DonutClosureProvider, DonutClosureObject> {
public:
	DonutClosureProvider( Handler<Heap> const& heap );
	virtual ~DonutClosureProvider() noexcept = default;
};

}
