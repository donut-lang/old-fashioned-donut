/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include "Donut.hpp"

namespace donut {
using namespace cinamo;

class Patron : public HandlerBody<Patron>
{
protected:
	Patron() = default;
	virtual ~Patron() noexcept = default;
public:
	inline bool onFree() const noexcept { return false; };
public:
	virtual void onRegisterProvider(Handler<Heap> const& heap) = 0;
	virtual void onGlobalObjectInitialized(Handler<Heap> const& heap) = 0;
};

}
