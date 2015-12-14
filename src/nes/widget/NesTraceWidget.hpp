/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <cinamo/Handler.h>
#include <cinamo/XVal.h>
#include "../../chisa/Hexe.hpp"
#include "../../chisa/tk/Widget.hpp"
#include "../../chisa/gl/PredefinedSymRenderer.hpp"

namespace nes {
using namespace cinamo;

class VirtualMachine;
class NesGeist;
struct Instruction;

class NesTraceWidget: public ::chisa::tk::Widget {
private:
	chisa::HandlerW<nes::NesGeist> geist_;
	chisa::gl::PredefinedSymRenderer asmRenderer_;
	chisa::gl::PredefinedSymRenderer numRenderer_;
	enum {
		Dollar    , // "$"
		RightParen, // "("
		LeftParen , // ")"
		SqRightParen, // "["
		SqLeftParen , // "]"
		Eq        , // "="
		Comma     , //","
		AtMark    , //"@"
		SymX      , //"X"
		SymY      , //"Y"
	};
	chisa::gl::PredefinedSymRenderer symRenderer_;
	float addrWidth_;
	int pcDelta_;
	uint16_t lastPC_;
	uint16_t lastDrawnPCStart_;
	uint16_t lastDrawnPCEnd_;
	float scrollIntegral_;
public:
	NesTraceWidget(Logger& log, chisa::HandlerW<chisa::tk::World> world, tinyxml2::XMLElement* element);
	virtual ~NesTraceWidget() noexcept = default;
private:
	void renderInst(chisa::gl::Canvas& cv, VirtualMachine& vm, uint16_t const& nowPC, Instruction const& inst, float const& rowWidth, float const& rowHeight, float const& offsetY);
	uint16_t ptToAddr(chisa::geom::Point const& pt);
	chisa::geom::Area addrToArea(uint16_t const& addr);
private:
	virtual void renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override;
	virtual void idleImpl(const float delta_ms) override;
	virtual void reshapeImpl(chisa::geom::Box const& areaSize) override;
	virtual chisa::geom::Box measureImpl(chisa::geom::Box const& constraintSize) override;
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
	virtual bool onScroll(float const& timeMs, chisa::geom::Point const& start, chisa::geom::Point const& end, chisa::geom::Distance const& distance) override final;
private:
	virtual bool onSingleTapUp(float const& timeMs, chisa::geom::Point const& ptInWidget) override;
};

}
