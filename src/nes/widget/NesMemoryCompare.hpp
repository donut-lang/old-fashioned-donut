/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../../chisa/tk/Widget.hpp"
#include "../../chisa/gl/Drawable.hpp"
#include "../../chisa/gl/PredefinedSymRenderer.hpp"

namespace nes {
using namespace cinamo;

class NesGeist;
class NesMemoryCompare final : public ::chisa::tk::Widget {
private:
	Handler<chisa::gl::TextDrawable> lAddr_; // "アドレス"
	Handler<chisa::gl::TextDrawable> lLast_; // "前の値"
	Handler<chisa::gl::TextDrawable> lNow_; // "今の値"
	chisa::gl::PredefinedSymRenderer numRenderer_;
	static const constexpr unsigned int Dollar=16;
	static const constexpr unsigned int HexStart=17;
	static const constexpr unsigned int Eq=18;
	static const constexpr unsigned int RParen=19;
	static const constexpr unsigned int LParen=20;
	static const constexpr unsigned int Space=21;
	float width_;
	float addrWidth_;
	float lastWidth_;
	float nowWidth_;
private:
	unsigned int lastCandidates_;
	chisa::HandlerW<nes::NesGeist> geist_;
public:
	NesMemoryCompare(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element);
	virtual ~NesMemoryCompare() noexcept = default;
private:
	inline float rowHeight() const noexcept { return this->numRenderer_.maxHeight(); };
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
private:
	chisa::geom::Area addrToArea(uint16_t const& addr);
	uint16_t ptToAddr(chisa::geom::Point const& pt);
private:
	virtual void renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area) override final;
	virtual void idleImpl(const float delta_ms) override final;
	virtual void reshapeImpl(chisa::geom::Box const& areaSize) override final;
	virtual chisa::geom::Box measureImpl(chisa::geom::Box const& constraintSize) override final;
private:
	virtual bool onSingleTapUp(float const& timeMs, chisa::geom::Point const& ptInWidget) override;
};

}
