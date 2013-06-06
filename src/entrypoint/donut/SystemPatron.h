/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <donut/Patron.h>
#include "SystemProvider.h"

namespace donut_cli {

class SystemPatron final : public donut::Patron {
	int argc_;
	char** argv_;
	Handler<SystemProvider> systemProvider_;
public:
	SystemPatron(int argc, char** argv);
	virtual ~SystemPatron() noexcept = default;
public:
	virtual void onRegisterProvider(Handler<Heap> const& heap) override final;
	virtual void onGlobalObjectInitialized(Handler<Heap> const& heap) override final;
};



}
