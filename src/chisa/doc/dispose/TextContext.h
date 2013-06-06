/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cstdint>
#include <cstddef>
#include <cinamo/Logger.h>
#include "../../gl/DrawableManager.h"
#include "../../geom/Vector.h"
#include "../../gl/Color.h"
#include "../render/RenderTree.h"

namespace chisa {
namespace doc {

class TextContext {
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	Handler<RenderTree> renderTree_;
	Handler<gl::Font> font_;
	std::vector<gl::TextDrawable::Style> styleStack_;
	std::vector<gl::TextDrawable::Decoration> decoStack_;
	std::vector<float> sizeStack_;
	std::vector<std::string> fontStack_;
	std::vector<gl::Color> colorStack_;
	std::vector<gl::Color> backColorStack_;
public:
	void pushStyle(gl::TextDrawable::Style style);
	gl::TextDrawable::Style nowStyle() const;
	void popStyle();

	void pushDeco(gl::TextDrawable::Decoration deco);
	gl::TextDrawable::Decoration nowDeco() const;
	void popDeco();

	void pushSize(float size);
	float nowSize() const;
	void popSize();

	void pushColor(gl::Color const& c);
	gl::Color nowColor() const;
	void popColor();

	void pushBackColor(gl::Color const& c);
	gl::Color nowBackColor() const;
	void popBackColor();

	void pushFont( std::string const& name );
	std::string nowFont() const;
	void popFont();
public:
	Handler<TextDrawableObject> create(std::string const& strUtf8, Node* parentNode, const float relDepth );
public:
	TextContext(Logger& log, Handler<RenderTree> renderTree);
	virtual ~TextContext() noexcept;
};

}}
