/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <vector>
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/Logger.h>
#include "internal/FontManager.hpp"

namespace chisa {
using namespace cinamo;
namespace gl {

class BitmapGlyph : public HandlerBody<BitmapGlyph> {
private:
	FT_BitmapGlyph glyph_;
public:
	BitmapGlyph(FT_BitmapGlyph glyph);
	~BitmapGlyph() noexcept;
	inline bool onFree() noexcept { return false; };
public:
	FT_BitmapGlyph get() const noexcept { return this->glyph_; };
};

class Font : public HandlerBody<Font>
{
public:
	enum Flag {
		Italic = 0,
		Bold = 1
	};
private:
	HandlerW<internal::FontManager> parent_;
	Handler<internal::FreeType> freetype_;
	FT_Face face_;
	DEFINE_MEMBER(public, private, unsigned int, unicodeCharmapIndex);
	DEFINE_MEMBER(private, private, bool, locked);
public:
	Font(internal::FontManager* parent, Handler<internal::FreeType> freetype, FT_Face face);
	virtual ~Font() noexcept;
public:
	std::string family() const noexcept;
	std::string style() const noexcept;
public:
	std::vector<Handler<BitmapGlyph> > lookupGlyph(std::string const& str, float size, float& ascent, float& descent, float& height) noexcept;
public:
	static void analyzeFontName(std::string const& name, std::string& family, std::string& style) noexcept;
public:
	class RawFaceSession {
		DISABLE_COPY_AND_ASSIGN(RawFaceSession);
		STACK_OBJECT(RawFaceSession);
	private:
		Font& parent_;
	public:
		RawFaceSession(Handler<Font> font):parent_(*font.get()){
			if(this->parent_.locked()){
				CINAMO_EXCEPTION(Exception, "[BUG] Failed to lock Font.");
			}
			this->parent_.locked(true);
		};
		RawFaceSession(Font& font):parent_(font){
			if(this->parent_.locked()){
				CINAMO_EXCEPTION(Exception, "[BUG] Failed to lock Font.");
			}
			this->parent_.locked(true);
		};
		~RawFaceSession() {
			if(!this->parent_.locked()){
				CINAMO_EXCEPTION(Exception, "[BUG] Oops. Font is already unlocked.");
			}
			this->parent_.locked(false);
		}
		FT_Face face() const noexcept { return parent_.face_; };
	};
public:
	bool onFree();
};

}}
