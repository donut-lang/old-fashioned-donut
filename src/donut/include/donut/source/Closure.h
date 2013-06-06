/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cinamo/Handler.h>
#include <cinamo/XVal.h>
#include "Inst.h"

namespace donut {
using namespace cinamo;

class Closure final : public HandlerBody<Closure>{
	const std::vector<std::string> arglist_;
	const std::vector<donut::Instruction> instlist_;
private:
	Closure(Closure const& other) = delete;
	Closure(Closure&& other) = delete;
	Closure& operator=(Closure const& other) = delete;
	Closure& operator=(Closure&& other) = delete;
public:
	Closure(std::vector<std::string> const& arglist, std::vector<donut::Instruction> const& instlist);
	Closure(XValue const& data);
	XValue save();
	virtual ~Closure() noexcept = default;
public:
	bool onFree() noexcept { return false; };
public:
	inline std::vector<std::string> const& arglist() const noexcept { return this->arglist_; };
	inline std::vector<donut::Instruction> const& instlist() const noexcept { return this->instlist_; };
public:
	bool operator==( Closure const& other ) const noexcept{
		return other.arglist_ == this->arglist_ && this->instlist_ == other.instlist_;
	}
	bool operator!=( Closure const& other ) const noexcept{
		return !this->operator ==(other);
	}
};

}
