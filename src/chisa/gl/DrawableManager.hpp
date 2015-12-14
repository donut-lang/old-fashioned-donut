/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/Logger.h>
#include <cinamo/VectorMap.h>
#include "Sprite.hpp"
#include "Font.hpp"
#include "internal/ImageManager.hpp"
#include "Drawable.hpp"
#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

#ifdef Handler
#undef Handler
#endif

namespace chisa {
using namespace cinamo;

namespace gl {

class DrawableSetting {
	DEFINE_MEMBER(public, public, std::string, fontdir);
public:
	DrawableSetting( DrawableSetting const& other) = default;
	DrawableSetting( DrawableSetting&& other) = default;
	DrawableSetting& operator=( DrawableSetting const& other) = default;
	DrawableSetting& operator=( DrawableSetting&& other) = default;
	~DrawableSetting() noexcept = default;
	DrawableSetting( std::string const& fontdir ):fontdir_(fontdir){};
};

//---------------------------------------------------------------------------------------------------------------------

class DrawableManager : public HandlerBody<DrawableManager> {
	DEFINE_MEMBER_REF(private, Logger, log);
	Canvas& canvas_;
private:
	typedef std::function<Handler<Drawable>(HandlerW<DrawableManager>, geom::Box const&, std::string const&)> constructor;
	VectorMap<std::string, constructor> factories_;
private:
	Handler<internal::SpriteManager> spriteManager_;
	Handler<internal::ImageManager> imageManager_;
	Handler<internal::FontManager> fontManager_;
public:
	DrawableManager(Logger& log, Canvas& canvas, DrawableSetting const& setting);
	virtual ~DrawableManager() noexcept = default;
public:
	Handler<Sprite> queryRawSprite(ImageFormat const format, const int width, const int height);
	int maxTextureSize() const noexcept;
	Handler<Sprite> queryImage(std::string const& filename);
	Handler<Drawable> queryDrawable(std::string const& repl, geom::Box const& box=geom::Box());
	Handler<Font> queryFont( std::string const& name = std::string() );
	Handler<TextDrawable> queryText(
			std::string const& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			gl::Color const& color=gl::Black,
			gl::Color const& backColor=gl::Transparent
		);
	Handler<TextDrawable> queryVerticalText(
			std::string const& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			gl::Color const& color=gl::Black,
			gl::Color const& backColor=gl::Transparent
		);
public:
	bool onFree() noexcept { return false; };
};

}}
