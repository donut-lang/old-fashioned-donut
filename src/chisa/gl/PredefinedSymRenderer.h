/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <unordered_map>
#include "../geom/Decl.h"
#include "DrawableManager.h"

namespace chisa {
using namespace cinamo;
namespace gl {
class Canvas;


class PredefinedSymRenderer {
public:
	typedef unsigned int Symbol;
	typedef std::vector<Symbol> SymList;
private:
	struct Entry {
		STACK_OBJECT(Entry);
		DEFAULT_COPY_AND_ASSIGN(Entry);
	public:
		Entry() = default;
		std::string str;
		std::vector<Handler<BitmapGlyph> > glyphs;
		geom::Area areaInSprite;
		unsigned int spriteNo;
	};
	struct Image{
		STACK_OBJECT(Image);
		DEFAULT_COPY_AND_ASSIGN(Image);
		Image() = default;
	public:
		Handler<Sprite> sprite;
		std::vector<float> vertexBuffer;
		std::vector<float> coordBuffer;
	public:
		void add(geom::Point const& pt, geom::Area const& spriteArea){
			Handler<Sprite> const& spr = this->sprite;
			//
			vertexBuffer.push_back(pt.x());vertexBuffer.push_back(pt.y());vertexBuffer.push_back(0);
			vertexBuffer.push_back(pt.x() + spriteArea.width());vertexBuffer.push_back(pt.y());vertexBuffer.push_back(0);
			vertexBuffer.push_back(pt.x());vertexBuffer.push_back(pt.y()+spriteArea.height());vertexBuffer.push_back(0);

			vertexBuffer.push_back(pt.x());vertexBuffer.push_back(pt.y()+spriteArea.height());vertexBuffer.push_back(0);
			vertexBuffer.push_back(pt.x() + spriteArea.width());vertexBuffer.push_back(pt.y());vertexBuffer.push_back(0);
			vertexBuffer.push_back(pt.x() + spriteArea.width());vertexBuffer.push_back(pt.y() +spriteArea.height() );vertexBuffer.push_back(0);

			//
			coordBuffer.push_back((spriteArea.x()                     ) / spr->width()); coordBuffer.push_back((spriteArea.y()                     )  / spr->height());
			coordBuffer.push_back((spriteArea.x() + spriteArea.width() ) / spr->width()); coordBuffer.push_back((spriteArea.y()                     )  / spr->height());
			coordBuffer.push_back((spriteArea.x()                     ) / spr->width()); coordBuffer.push_back((spriteArea.y() + spriteArea.height() ) / spr->height());

			coordBuffer.push_back((spriteArea.x()                    ) / spr->width() ); coordBuffer.push_back((spriteArea.y() + spriteArea.height()) / spr->height());
			coordBuffer.push_back((spriteArea.x() + spriteArea.width()) / spr->width() ); coordBuffer.push_back((spriteArea.y()                     ) / spr->height());
			coordBuffer.push_back((spriteArea.x() + spriteArea.width()) / spr->width() ); coordBuffer.push_back((spriteArea.y() + spriteArea.height()) / spr->height());
}
	};
private:
	DISABLE_COPY_AND_ASSIGN(PredefinedSymRenderer);
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	Handler<DrawableManager> drawableManager_;
	std::unordered_map<unsigned int, Entry > entryTable_;
	std::vector<Image> renderBuffer_;
private:
	Handler<gl::Font> font_;
	float size_;
	gl::Color color_;
	float ascent_;
	float descent_;
	float height_;
	float maxWidth_;
	gl::Color backgroundColor_;
public:
	PredefinedSymRenderer(Logger& log, Handler<DrawableManager> drawableManager, float size);
	virtual ~PredefinedSymRenderer() noexcept = default;
public:
	void registerSymbol( Symbol symbol, std::string const& str );
	void compile();
	void flush( Canvas& cv );
	inline float const& maxWidth() const noexcept { return this->maxWidth_; };
	inline float const& maxHeight() const noexcept { return this->height_; };
	geom::Area renderSyms( Canvas& cv, geom::Point const& point, SymList const& str, float depth );
	geom::Area renderSym( Canvas& cv, geom::Point const& point, Symbol const& str, float depth );
};

}}
