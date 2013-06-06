/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <donut/Donut.h>
#include <donut/provider/Provider.h>

namespace donut_cli {
using namespace donut;

class SystemObject;
class SystemProvider final : public ReactiveProviderBaseT<SystemProvider, SystemObject> {
private:
	std::vector<std::string> const args_;
public:
	SystemProvider(Handler<Heap> const& heap, int argc, char** argv);
	virtual ~SystemProvider() noexcept = default;
public:
	inline std::vector<std::string> const& args() const noexcept{ return this->args_; };
};

}
